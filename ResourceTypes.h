/*
 * Copyright (C) 2005 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RESOURCE_TYPES_H
#define RESOURCE_TYPES_H

#include "configuration.h"

#include <string>
#include <stdint.h>
#include <memory.h>

#define Res_MAKEINTERNAL(entry) (0x01000000 | (entry&0xFFFF))

enum {
    RES_NULL_TYPE               = 0x0000,
    RES_STRING_POOL_TYPE        = 0x0001,
    RES_TABLE_TYPE              = 0x0002,
    RES_XML_TYPE                = 0x0003,

    // Chunk types in RES_XML_TYPE
    RES_XML_FIRST_CHUNK_TYPE    = 0x0100,
    RES_XML_START_NAMESPACE_TYPE= 0x0100,
    RES_XML_END_NAMESPACE_TYPE  = 0x0101,
    RES_XML_START_ELEMENT_TYPE  = 0x0102,
    RES_XML_END_ELEMENT_TYPE    = 0x0103,
    RES_XML_CDATA_TYPE          = 0x0104,
    RES_XML_LAST_CHUNK_TYPE     = 0x017f,
    // This contains a uint32_t array mapping strings in the string
    // pool back to resource identifiers.  It is optional.
    RES_XML_RESOURCE_MAP_TYPE   = 0x0180,

    // Chunk types in RES_TABLE_TYPE
    RES_TABLE_PACKAGE_TYPE      = 0x0200,
    RES_TABLE_TYPE_TYPE         = 0x0201,
    RES_TABLE_TYPE_SPEC_TYPE    = 0x0202,
    RES_TABLE_LIBRARY_TYPE      = 0x0203
};


/**
* Header that appears at the front of every data chunk in a resource.
*/
struct ResChunk_header
{
   // Type identifier for this chunk.  The meaning of this value depends
   // on the containing chunk.
   uint16_t type;

   // Size of the chunk header (in bytes).  Adding this value to
   // the address of the chunk allows you to find its associated data
   // (if any).
   uint16_t headerSize;

   // Total size of this chunk (in bytes).  This is the chunkSize plus
   // the size of any data associated with the chunk.  Adding this value
   // to the chunk allows you to completely skip its contents (including
   // any child chunks).  If this value is the same as chunkSize, there is
   // no data associated with the chunk.
   uint32_t size;
};

/**
 * Definition for a pool of strings.  The data of this chunk is an
 * array of uint32_t providing indices into the pool, relative to
 * stringsStart.  At stringsStart are all of the UTF-16 strings
 * concatenated together; each starts with a uint16_t of the string's
 * length and each ends with a 0x0000 terminator.  If a string is >
 * 32767 characters, the high bit of the length is set meaning to take
 * those 15 bits as a high word and it will be followed by another
 * uint16_t containing the low word.
 *
 * If styleCount is not zero, then immediately following the array of
 * uint32_t indices into the string table is another array of indices
 * into a style table starting at stylesStart.  Each entry in the
 * style table is an array of ResStringPool_span structures.
 */
struct ResStringPool_header
{
    struct ResChunk_header header;

    // Number of strings in this pool (number of uint32_t indices that follow
    // in the data).
    uint32_t stringCount;

    // Number of style span arrays in the pool (number of uint32_t indices
    // follow the string indices).
    uint32_t styleCount;

    // Flags.
    enum {
        // If set, the string index is sorted by the string values (based
        // on strcmp16()).
        SORTED_FLAG = 1<<0,

        // String pool is encoded in UTF-8
        UTF8_FLAG = 1<<8
    };
    uint32_t flags;

    // Index from header of the string data.
    uint32_t stringsStart;

    // Index from header of the style data.
    uint32_t stylesStart;
};


/**
 * Header for a resource table.  Its data contains a series of
 * additional chunks:
 *   * A ResStringPool_header containing all table values.  This string pool
 *     contains all of the string values in the entire resource table (not
 *     the names of entries or type identifiers however).
 *   * One or more ResTable_package chunks.
 *
 * Specific entries within a resource table can be uniquely identified
 * with a single integer as defined by the ResTable_ref structure.
 */
struct ResTable_header
{
    struct ResChunk_header header;

    // The number of ResTable_package structures.
    uint32_t packageCount;
};

/**
 * XML tree header.  This appears at the front of an XML tree,
 * describing its content.  It is followed by a flat array of
 * ResXMLTree_node structures; the hierarchy of the XML document
 * is described by the occurrance of RES_XML_START_ELEMENT_TYPE
 * and corresponding RES_XML_END_ELEMENT_TYPE nodes in the array.
 */
struct ResXMLTree_header
{
    struct ResChunk_header header;
};

/**
 * A collection of resource data types within a package.  Followed by
 * one or more ResTable_type and ResTable_typeSpec structures containing the
 * entry values for each resource type.
 */
struct ResTable_package
{
    struct ResChunk_header header;

    // If this is a base package, its ID.  Package IDs start
    // at 1 (corresponding to the value of the package bits in a
    // resource identifier).  0 means this is not a base package.
    uint32_t id;

    // Actual name of this package, \0-terminated.
    uint16_t name[128];

    // Offset to a ResStringPool_header defining the resource
    // type symbol table.  If zero, this package is inheriting from
    // another base package (overriding specific values in it).
    uint32_t typeStrings;

    // Last index into typeStrings that is for public use by others.
    uint32_t lastPublicType;

    // Offset to a ResStringPool_header defining the resource
    // key symbol table.  If zero, this package is inheriting from
    // another base package (overriding specific values in it).
    uint32_t keyStrings;

    // Last index into keyStrings that is for public use by others.
    uint32_t lastPublicKey;

    uint32_t typeIdOffset;
};

/**
 * A specification of the resources defined by a particular type.
 *
 * There should be one of these chunks for each resource type.
 *
 * This structure is followed by an array of integers providing the set of
 * configuration change flags (ResTable_config::CONFIG_*) that have multiple
 * resources for that configuration.  In addition, the high bit is set if that
 * resource has been made public.
 */
struct ResTable_typeSpec
{
    struct ResChunk_header header;

    // The type identifier this chunk is holding.  Type IDs start
    // at 1 (corresponding to the value of the type bits in a
    // resource identifier).  0 is invalid.
    uint8_t id;
    
    // Must be 0.
    uint8_t res0;
    // Must be 0.
    uint16_t res1;
    
    // Number of uint32_t entry configuration masks that follow.
    uint32_t entryCount;

    enum {
        // Additional flag indicating an entry is public.
        SPEC_PUBLIC = 0x40000000
    };
};

struct ResTable_config
{
	// Number of bytes in this structure.
    uint32_t size;

    union {
        struct {
            // Mobile country code (from SIM).  0 means "any".
            uint16_t mcc;
            // Mobile network code (from SIM).  0 means "any".
            uint16_t mnc;
        };
        uint32_t imsi;
    };
    
    union {
        struct {
            // \0\0 means "any".  Otherwise, en, fr, etc.
            char language[2];
            
            // \0\0 means "any".  Otherwise, US, CA, etc.
            char country[2];
        };
        uint32_t locale;
    };
    
    enum {
        ORIENTATION_ANY  = ACONFIGURATION_ORIENTATION_ANY,
        ORIENTATION_PORT = ACONFIGURATION_ORIENTATION_PORT,
        ORIENTATION_LAND = ACONFIGURATION_ORIENTATION_LAND,
        ORIENTATION_SQUARE = ACONFIGURATION_ORIENTATION_SQUARE,
    };
    
    enum {
        TOUCHSCREEN_ANY  = ACONFIGURATION_TOUCHSCREEN_ANY,
        TOUCHSCREEN_NOTOUCH  = ACONFIGURATION_TOUCHSCREEN_NOTOUCH,
        TOUCHSCREEN_STYLUS  = ACONFIGURATION_TOUCHSCREEN_STYLUS,
        TOUCHSCREEN_FINGER  = ACONFIGURATION_TOUCHSCREEN_FINGER,
    };
    
    enum {
        DENSITY_DEFAULT = ACONFIGURATION_DENSITY_DEFAULT,
        DENSITY_LOW = ACONFIGURATION_DENSITY_LOW,
        DENSITY_MEDIUM = ACONFIGURATION_DENSITY_MEDIUM,
        DENSITY_TV = ACONFIGURATION_DENSITY_TV,
        DENSITY_HIGH = ACONFIGURATION_DENSITY_HIGH,
        DENSITY_XHIGH = ACONFIGURATION_DENSITY_XHIGH,
        DENSITY_XXHIGH = ACONFIGURATION_DENSITY_XXHIGH,
        DENSITY_XXXHIGH = ACONFIGURATION_DENSITY_XXXHIGH,
        DENSITY_NONE = ACONFIGURATION_DENSITY_NONE
    };
    
    union {
        struct {
            uint8_t orientation;
            uint8_t touchscreen;
            uint16_t density;
        };
        uint32_t screenType;
    };
    
    enum {
        KEYBOARD_ANY  = ACONFIGURATION_KEYBOARD_ANY,
        KEYBOARD_NOKEYS  = ACONFIGURATION_KEYBOARD_NOKEYS,
        KEYBOARD_QWERTY  = ACONFIGURATION_KEYBOARD_QWERTY,
        KEYBOARD_12KEY  = ACONFIGURATION_KEYBOARD_12KEY,
    };
    
    enum {
        NAVIGATION_ANY  = ACONFIGURATION_NAVIGATION_ANY,
        NAVIGATION_NONAV  = ACONFIGURATION_NAVIGATION_NONAV,
        NAVIGATION_DPAD  = ACONFIGURATION_NAVIGATION_DPAD,
        NAVIGATION_TRACKBALL  = ACONFIGURATION_NAVIGATION_TRACKBALL,
        NAVIGATION_WHEEL  = ACONFIGURATION_NAVIGATION_WHEEL,
    };
    
    enum {
        MASK_KEYSHIDDEN = 0x0003,
        KEYSHIDDEN_ANY = ACONFIGURATION_KEYSHIDDEN_ANY,
        KEYSHIDDEN_NO = ACONFIGURATION_KEYSHIDDEN_NO,
        KEYSHIDDEN_YES = ACONFIGURATION_KEYSHIDDEN_YES,
        KEYSHIDDEN_SOFT = ACONFIGURATION_KEYSHIDDEN_SOFT,
    };
    
    enum {
        MASK_NAVHIDDEN = 0x000c,
        SHIFT_NAVHIDDEN = 2,
        NAVHIDDEN_ANY = ACONFIGURATION_NAVHIDDEN_ANY << SHIFT_NAVHIDDEN,
        NAVHIDDEN_NO = ACONFIGURATION_NAVHIDDEN_NO << SHIFT_NAVHIDDEN,
        NAVHIDDEN_YES = ACONFIGURATION_NAVHIDDEN_YES << SHIFT_NAVHIDDEN,
    };
    
    union {
        struct {
            uint8_t keyboard;
            uint8_t navigation;
            uint8_t inputFlags;
            uint8_t inputPad0;
        };
        uint32_t input;
    };
    
    enum {
        SCREENWIDTH_ANY = 0
    };
    
    enum {
        SCREENHEIGHT_ANY = 0
    };
    
    union {
        struct {
            uint16_t screenWidth;
            uint16_t screenHeight;
        };
        uint32_t screenSize;
    };
    
    enum {
        SDKVERSION_ANY = 0
    };
    
    enum {
        MINORVERSION_ANY = 0
    };
    
    union {
        struct {
            uint16_t sdkVersion;
            // For now minorVersion must always be 0!!!  Its meaning
            // is currently undefined.
            uint16_t minorVersion;
        };
        uint32_t version;
    };
    
    enum {
        // screenLayout bits for screen size class.
        MASK_SCREENSIZE = 0x0f,
        SCREENSIZE_ANY = ACONFIGURATION_SCREENSIZE_ANY,
        SCREENSIZE_SMALL = ACONFIGURATION_SCREENSIZE_SMALL,
        SCREENSIZE_NORMAL = ACONFIGURATION_SCREENSIZE_NORMAL,
        SCREENSIZE_LARGE = ACONFIGURATION_SCREENSIZE_LARGE,
        SCREENSIZE_XLARGE = ACONFIGURATION_SCREENSIZE_XLARGE,
        
        // screenLayout bits for wide/long screen variation.
        MASK_SCREENLONG = 0x30,
        SHIFT_SCREENLONG = 4,
        SCREENLONG_ANY = ACONFIGURATION_SCREENLONG_ANY << SHIFT_SCREENLONG,
        SCREENLONG_NO = ACONFIGURATION_SCREENLONG_NO << SHIFT_SCREENLONG,
        SCREENLONG_YES = ACONFIGURATION_SCREENLONG_YES << SHIFT_SCREENLONG,

        // screenLayout bits for layout direction.
        MASK_LAYOUTDIR = 0xC0,
        SHIFT_LAYOUTDIR = 6,
        LAYOUTDIR_ANY = ACONFIGURATION_LAYOUTDIR_ANY << SHIFT_LAYOUTDIR,
        LAYOUTDIR_LTR = ACONFIGURATION_LAYOUTDIR_LTR << SHIFT_LAYOUTDIR,
        LAYOUTDIR_RTL = ACONFIGURATION_LAYOUTDIR_RTL << SHIFT_LAYOUTDIR,
    };
    
    enum {
        // uiMode bits for the mode type.
        MASK_UI_MODE_TYPE = 0x0f,
        UI_MODE_TYPE_ANY = ACONFIGURATION_UI_MODE_TYPE_ANY,
        UI_MODE_TYPE_NORMAL = ACONFIGURATION_UI_MODE_TYPE_NORMAL,
        UI_MODE_TYPE_DESK = ACONFIGURATION_UI_MODE_TYPE_DESK,
        UI_MODE_TYPE_CAR = ACONFIGURATION_UI_MODE_TYPE_CAR,
        UI_MODE_TYPE_TELEVISION = ACONFIGURATION_UI_MODE_TYPE_TELEVISION,
        UI_MODE_TYPE_APPLIANCE = ACONFIGURATION_UI_MODE_TYPE_APPLIANCE,

        // uiMode bits for the night switch.
        MASK_UI_MODE_NIGHT = 0x30,
        SHIFT_UI_MODE_NIGHT = 4,
        UI_MODE_NIGHT_ANY = ACONFIGURATION_UI_MODE_NIGHT_ANY << SHIFT_UI_MODE_NIGHT,
        UI_MODE_NIGHT_NO = ACONFIGURATION_UI_MODE_NIGHT_NO << SHIFT_UI_MODE_NIGHT,
        UI_MODE_NIGHT_YES = ACONFIGURATION_UI_MODE_NIGHT_YES << SHIFT_UI_MODE_NIGHT,
    };

    union {
        struct {
            uint8_t screenLayout;
            uint8_t uiMode;
            uint16_t smallestScreenWidthDp;
        };
        uint32_t screenConfig;
    };
    
    union {
        struct {
            uint16_t screenWidthDp;
            uint16_t screenHeightDp;
        };
        uint32_t screenSizeDp;
    };

	// Flags indicating a set of config values.  These flag constants must
    // match the corresponding ones in android.content.pm.ActivityInfo and
    // attrs_manifest.xml.
    enum {
        CONFIG_MCC = ACONFIGURATION_MCC,
        CONFIG_MNC = ACONFIGURATION_MCC,
        CONFIG_LOCALE = ACONFIGURATION_LOCALE,
        CONFIG_TOUCHSCREEN = ACONFIGURATION_TOUCHSCREEN,
        CONFIG_KEYBOARD = ACONFIGURATION_KEYBOARD,
        CONFIG_KEYBOARD_HIDDEN = ACONFIGURATION_KEYBOARD_HIDDEN,
        CONFIG_NAVIGATION = ACONFIGURATION_NAVIGATION,
        CONFIG_ORIENTATION = ACONFIGURATION_ORIENTATION,
        CONFIG_DENSITY = ACONFIGURATION_DENSITY,
        CONFIG_SCREEN_SIZE = ACONFIGURATION_SCREEN_SIZE,
        CONFIG_SMALLEST_SCREEN_SIZE = ACONFIGURATION_SMALLEST_SCREEN_SIZE,
        CONFIG_VERSION = ACONFIGURATION_VERSION,
        CONFIG_SCREEN_LAYOUT = ACONFIGURATION_SCREEN_LAYOUT,
        CONFIG_UI_MODE = ACONFIGURATION_UI_MODE,
        CONFIG_LAYOUTDIR = ACONFIGURATION_LAYOUTDIR,
    };

	std::string toString() const;
};
    

struct ResTable_type
{
    struct ResChunk_header header;

    enum {
        NO_ENTRY = 0xFFFFFFFF
    };
    
    // The type identifier this chunk is holding.  Type IDs start
    // at 1 (corresponding to the value of the type bits in a
    // resource identifier).  0 is invalid.
    uint8_t id;
    
    // Must be 0.
    uint8_t res0;
    // Must be 0.
    uint16_t res1;
    
    // Number of uint32_t entry indices that follow.
    uint32_t entryCount;

    // Offset from header where ResTable_entry data starts.
    uint32_t entriesStart;

	ResTable_config config;
};

/**
 * Reference to a string in a string pool.
 */
struct ResStringPool_ref
{
    // Index into the string pool table (uint32_t-offset from the indices
    // immediately after ResStringPool_header) at which to find the location
    // of the string data in the pool.
    uint32_t index;
};


/**
 * This is the beginning of information about an entry in the resource
 * table.  It holds the reference to the name of this entry, and is
 * immediately followed by one of:
 *   * A Res_value structure, if FLAG_COMPLEX is -not- set.
 *   * An array of ResTable_map structures, if FLAG_COMPLEX is set.
 *     These supply a set of name/value mappings of data.
 */
struct ResTable_entry
{
    // Number of bytes in this structure.
    uint16_t size;

    enum {
        // If set, this is a complex entry, holding a set of name/value
        // mappings.  It is followed by an array of ResTable_map structures.
        FLAG_COMPLEX = 0x0001,
        // If set, this resource has been declared public, so libraries
        // are allowed to reference it.
        FLAG_PUBLIC = 0x0002,
        // If set, this is a weak resource and may be overriden by strong
        // resources of the same name/type. This is only useful during
        // linking with other resource tables.
        FLAG_WEAK = 0x0004
    };
    uint16_t flags;
    
    // Reference into ResTable_package::keyStrings identifying this entry.
    struct ResStringPool_ref key;
};

/**
 *  This is a reference to a unique entry (a ResTable_entry structure)
 *  in a resource table.  The value is structured as: 0xpptteeee,
 *  where pp is the package index, tt is the type index in that
 *  package, and eeee is the entry index in that type.  The package
 *  and type values start at 1 for the first item, to help catch cases
 *  where they have not been supplied.
 */
struct ResTable_ref
{
    uint32_t ident;
};

/**
 * Extended form of a ResTable_entry for map entries, defining a parent map
 * resource from which to inherit values.
 */
struct ResTable_map_entry : public ResTable_entry
{
    // Resource identifier of the parent mapping, or 0 if there is none.
    ResTable_ref parent;
    // Number of name/value pairs that follow for FLAG_COMPLEX.
    uint32_t count;
};

/**
 * Representation of a value in a resource, supplying type
 * information.
 */
struct Res_value
{
    // Number of bytes in this structure.
    uint16_t size;

    // Always set to 0.
    uint8_t res0;
        
    // Type of the data value.
    enum {
        // The 'data' is either 0 or 1, specifying this resource is either
        // undefined or empty, respectively.
        TYPE_NULL = 0x00,
        // The 'data' holds a ResTable_ref, a reference to another resource
        // table entry.
        TYPE_REFERENCE = 0x01,
        // The 'data' holds an attribute resource identifier.
        TYPE_ATTRIBUTE = 0x02,
        // The 'data' holds an index into the containing resource table's
        // global value string pool.
        TYPE_STRING = 0x03,
        // The 'data' holds a single-precision floating point number.
        TYPE_FLOAT = 0x04,
        // The 'data' holds a complex number encoding a dimension value,
        // such as "100in".
        TYPE_DIMENSION = 0x05,
        // The 'data' holds a complex number encoding a fraction of a
        // container.
        TYPE_FRACTION = 0x06,
        // The 'data' holds a dynamic ResTable_ref, which needs to be
        // resolved before it can be used like a TYPE_REFERENCE.
        TYPE_DYNAMIC_REFERENCE = 0x07,
        // The 'data' holds an attribute resource identifier, which needs to be resolved
        // before it can be used like a TYPE_ATTRIBUTE.
        TYPE_DYNAMIC_ATTRIBUTE = 0x08,

        // Beginning of integer flavors...
        TYPE_FIRST_INT = 0x10,

        // The 'data' is a raw integer value of the form n..n.
        TYPE_INT_DEC = 0x10,
        // The 'data' is a raw integer value of the form 0xn..n.
        TYPE_INT_HEX = 0x11,
        // The 'data' is either 0 or 1, for input "false" or "true" respectively.
        TYPE_INT_BOOLEAN = 0x12,

        // Beginning of color integer flavors...
        TYPE_FIRST_COLOR_INT = 0x1c,

        // The 'data' is a raw integer value of the form #aarrggbb.
        TYPE_INT_COLOR_ARGB8 = 0x1c,
        // The 'data' is a raw integer value of the form #rrggbb.
        TYPE_INT_COLOR_RGB8 = 0x1d,
        // The 'data' is a raw integer value of the form #argb.
        TYPE_INT_COLOR_ARGB4 = 0x1e,
        // The 'data' is a raw integer value of the form #rgb.
        TYPE_INT_COLOR_RGB4 = 0x1f,

        // ...end of integer flavors.
        TYPE_LAST_COLOR_INT = 0x1f,

        // ...end of integer flavors.
        TYPE_LAST_INT = 0x1f
    };
    uint8_t dataType;

    // Structure of complex data values (TYPE_UNIT and TYPE_FRACTION)
    enum {
        // Where the unit type information is.  This gives us 16 possible
        // types, as defined below.
        COMPLEX_UNIT_SHIFT = 0,
        COMPLEX_UNIT_MASK = 0xf,

        // TYPE_DIMENSION: Value is raw pixels.
        COMPLEX_UNIT_PX = 0,
        // TYPE_DIMENSION: Value is Device Independent Pixels.
        COMPLEX_UNIT_DIP = 1,
        // TYPE_DIMENSION: Value is a Scaled device independent Pixels.
        COMPLEX_UNIT_SP = 2,
        // TYPE_DIMENSION: Value is in points.
        COMPLEX_UNIT_PT = 3,
        // TYPE_DIMENSION: Value is in inches.
        COMPLEX_UNIT_IN = 4,
        // TYPE_DIMENSION: Value is in millimeters.
        COMPLEX_UNIT_MM = 5,

        // TYPE_FRACTION: A basic fraction of the overall size.
        COMPLEX_UNIT_FRACTION = 0,
        // TYPE_FRACTION: A fraction of the parent size.
        COMPLEX_UNIT_FRACTION_PARENT = 1,

        // Where the radix information is, telling where the decimal place
        // appears in the mantissa.  This give us 4 possible fixed point
        // representations as defined below.
        COMPLEX_RADIX_SHIFT = 4,
        COMPLEX_RADIX_MASK = 0x3,

        // The mantissa is an integral number -- i.e., 0xnnnnnn.0
        COMPLEX_RADIX_23p0 = 0,
        // The mantissa magnitude is 16 bits -- i.e, 0xnnnn.nn
        COMPLEX_RADIX_16p7 = 1,
        // The mantissa magnitude is 8 bits -- i.e, 0xnn.nnnn
        COMPLEX_RADIX_8p15 = 2,
        // The mantissa magnitude is 0 bits -- i.e, 0x0.nnnnnn
        COMPLEX_RADIX_0p23 = 3,

        // Where the actual value is.  This gives us 23 bits of
        // precision.  The top bit is the sign.
        COMPLEX_MANTISSA_SHIFT = 8,
        COMPLEX_MANTISSA_MASK = 0xffffff
    };

    // Possible data values for TYPE_NULL.
    enum {
        // The value is not defined.
        DATA_NULL_UNDEFINED = 0,
        // The value is explicitly defined as empty.
        DATA_NULL_EMPTY = 1
    };

    // The data for this item, as interpreted according to dataType.
    typedef uint32_t data_type;
    data_type data;
};

/**
 * A single name/value mapping that is part of a complex resource
 * entry.
 */
struct ResTable_map
{
    // The resource identifier defining this mapping's name.  For attribute
    // resources, 'name' can be one of the following special resource types
    // to supply meta-data about the attribute; for all other resource types
    // it must be an attribute resource.
    ResTable_ref name;

    // Special values for 'name' when defining attribute resources.
    enum {
        // This entry holds the attribute's type code.
        ATTR_TYPE = Res_MAKEINTERNAL(0),

        // For integral attributes, this is the minimum value it can hold.
        ATTR_MIN = Res_MAKEINTERNAL(1),

        // For integral attributes, this is the maximum value it can hold.
        ATTR_MAX = Res_MAKEINTERNAL(2),

        // Localization of this resource is can be encouraged or required with
        // an aapt flag if this is set
        ATTR_L10N = Res_MAKEINTERNAL(3),

        // for plural support, see android.content.res.PluralRules#attrForQuantity(int)
        ATTR_OTHER = Res_MAKEINTERNAL(4),
        ATTR_ZERO = Res_MAKEINTERNAL(5),
        ATTR_ONE = Res_MAKEINTERNAL(6),
        ATTR_TWO = Res_MAKEINTERNAL(7),
        ATTR_FEW = Res_MAKEINTERNAL(8),
        ATTR_MANY = Res_MAKEINTERNAL(9)
        
    };

    // Bit mask of allowed types, for use with ATTR_TYPE.
    enum {
        // No type has been defined for this attribute, use generic
        // type handling.  The low 16 bits are for types that can be
        // handled generically; the upper 16 require additional information
        // in the bag so can not be handled generically for TYPE_ANY.
        TYPE_ANY = 0x0000FFFF,

        // Attribute holds a references to another resource.
        TYPE_REFERENCE = 1<<0,

        // Attribute holds a generic string.
        TYPE_STRING = 1<<1,

        // Attribute holds an integer value.  ATTR_MIN and ATTR_MIN can
        // optionally specify a constrained range of possible integer values.
        TYPE_INTEGER = 1<<2,

        // Attribute holds a boolean integer.
        TYPE_BOOLEAN = 1<<3,

        // Attribute holds a color value.
        TYPE_COLOR = 1<<4,

        // Attribute holds a floating point value.
        TYPE_FLOAT = 1<<5,

        // Attribute holds a dimension value, such as "20px".
        TYPE_DIMENSION = 1<<6,

        // Attribute holds a fraction value, such as "20%".
        TYPE_FRACTION = 1<<7,

        // Attribute holds an enumeration.  The enumeration values are
        // supplied as additional entries in the map.
        TYPE_ENUM = 1<<16,

        // Attribute holds a bitmaks of flags.  The flag bit values are
        // supplied as additional entries in the map.
        TYPE_FLAGS = 1<<17
    };

    // Enum of localization modes, for use with ATTR_L10N.
    enum {
        L10N_NOT_REQUIRED = 0,
        L10N_SUGGESTED    = 1
    };
    
    // This mapping's value.
    Res_value value;
};


#endif  /*RESOURCE_TYPES_H*/
