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

#include "ResourceTypes.h"
#include "ByteOrder.h"

#include "sstream"

using namespace std;

class String {
public:
	int size() {
		return mStringStream.str().length();
	}

	String& append(long number) {
		mStringStream<<number;
		return *this;
	}

	String& append(const string& str) {
		mStringStream<<str;
		return *this;
	}

	String& append(const char* str, int length) {
		for(int i = 0 ; i < length ; i++) {
			mStringStream<<str[i];
		}
		return *this;
	}

	string toStdString() {
		return mStringStream.str();
	}

private:
	stringstream mStringStream;
};

string ResTable_config::toString() const {
    String res;

    if (mcc != 0) {
        if (res.size() > 0) res.append("-");
        res.append(dtohs(mcc)).append("mcc");
    }
    if (mnc != 0) {
        if (res.size() > 0) res.append("-");
        res.append(dtohs(mnc)).append("mnc");
    }
    if (language[0] != 0) {
        if (res.size() > 0) res.append("-");
        res.append(language, 2);
    }
    if (country[0] != 0) {
        if (res.size() > 0) res.append("-");
        res.append(country, 2);
    }
    if ((screenLayout&MASK_LAYOUTDIR) != 0) {
        if (res.size() > 0) res.append("-");
        switch (screenLayout&ResTable_config::MASK_LAYOUTDIR) {
            case ResTable_config::LAYOUTDIR_LTR:
                res.append("ldltr");
                break;
            case ResTable_config::LAYOUTDIR_RTL:
                res.append("ldrtl");
                break;
            default:
                res.append("layoutDir=")
					.append(dtohs(screenLayout&ResTable_config::MASK_LAYOUTDIR));
                break;
        }
    }
    if (smallestScreenWidthDp != 0) {
        if (res.size() > 0) res.append("-");
        res.append("sw").append(dtohs(smallestScreenWidthDp)).append("dp");
    }
    if (screenWidthDp != 0) {
        if (res.size() > 0) res.append("-");
        res.append("w").append(dtohs(screenWidthDp)).append("dp");
    }
    if (screenHeightDp != 0) {
        if (res.size() > 0) res.append("-");
        res.append("h").append(dtohs(screenHeightDp)).append("dp");
    }
    if ((screenLayout&MASK_SCREENSIZE) != SCREENSIZE_ANY) {
        if (res.size() > 0) res.append("-");
        switch (screenLayout&ResTable_config::MASK_SCREENSIZE) {
            case ResTable_config::SCREENSIZE_SMALL:
                res.append("small");
                break;
            case ResTable_config::SCREENSIZE_NORMAL:
                res.append("normal");
                break;
            case ResTable_config::SCREENSIZE_LARGE:
                res.append("large");
                break;
            case ResTable_config::SCREENSIZE_XLARGE:
                res.append("xlarge");
                break;
            default:
                res.append("screenLayoutSize=")
                    .append(dtohs(screenLayout&ResTable_config::MASK_SCREENSIZE));
                break;
        }
    }
    if ((screenLayout&MASK_SCREENLONG) != 0) {
        if (res.size() > 0) res.append("-");
        switch (screenLayout&ResTable_config::MASK_SCREENLONG) {
            case ResTable_config::SCREENLONG_NO:
                res.append("notlong");
                break;
            case ResTable_config::SCREENLONG_YES:
                res.append("long");
                break;
            default:
                res.append("screenLayoutLong=")
                        .append(dtohs(screenLayout&ResTable_config::MASK_SCREENLONG));
                break;
        }
    }
    if (orientation != ORIENTATION_ANY) {
        if (res.size() > 0) res.append("-");
        switch (orientation) {
            case ResTable_config::ORIENTATION_PORT:
                res.append("port");
                break;
            case ResTable_config::ORIENTATION_LAND:
                res.append("land");
                break;
            case ResTable_config::ORIENTATION_SQUARE:
                res.append("square");
                break;
            default:
                res.append("orientation=").append(dtohs(orientation));
                break;
        }
    }
    if ((uiMode&MASK_UI_MODE_TYPE) != UI_MODE_TYPE_ANY) {
        if (res.size() > 0) res.append("-");
        switch (uiMode&ResTable_config::MASK_UI_MODE_TYPE) {
            case ResTable_config::UI_MODE_TYPE_DESK:
                res.append("desk");
                break;
            case ResTable_config::UI_MODE_TYPE_CAR:
                res.append("car");
                break;
            case ResTable_config::UI_MODE_TYPE_TELEVISION:
                res.append("television");
                break;
            case ResTable_config::UI_MODE_TYPE_APPLIANCE:
                res.append("appliance");
                break;
            default:
                res.append("uiModeType=")
                    .append(dtohs(screenLayout&ResTable_config::MASK_UI_MODE_TYPE));
                break;
        }
    }
    if ((uiMode&MASK_UI_MODE_NIGHT) != 0) {
        if (res.size() > 0) res.append("-");
        switch (uiMode&ResTable_config::MASK_UI_MODE_NIGHT) {
            case ResTable_config::UI_MODE_NIGHT_NO:
                res.append("notnight");
                break;
            case ResTable_config::UI_MODE_NIGHT_YES:
                res.append("night");
                break;
            default:
                res.append("uiModeNight=")
                        .append(dtohs(uiMode&MASK_UI_MODE_NIGHT));
                break;
        }
    }
    if (density != DENSITY_DEFAULT) {
        if (res.size() > 0) res.append("-");
        switch (density) {
            case ResTable_config::DENSITY_LOW:
                res.append("ldpi");
                break;
            case ResTable_config::DENSITY_MEDIUM:
                res.append("mdpi");
                break;
            case ResTable_config::DENSITY_TV:
                res.append("tvdpi");
                break;
            case ResTable_config::DENSITY_HIGH:
                res.append("hdpi");
                break;
            case ResTable_config::DENSITY_XHIGH:
                res.append("xhdpi");
                break;
            case ResTable_config::DENSITY_XXHIGH:
                res.append("xxhdpi");
                break;
            case ResTable_config::DENSITY_NONE:
                res.append("nodpi");
                break;
            default:
                res.append(dtohs(density)).append("dpi");
                break;
        }
    }
    if (touchscreen != TOUCHSCREEN_ANY) {
        if (res.size() > 0) res.append("-");
        switch (touchscreen) {
            case ResTable_config::TOUCHSCREEN_NOTOUCH:
                res.append("notouch");
                break;
            case ResTable_config::TOUCHSCREEN_FINGER:
                res.append("finger");
                break;
            case ResTable_config::TOUCHSCREEN_STYLUS:
                res.append("stylus");
                break;
            default:
                res.append("touchscreen=").append(dtohs(touchscreen));
                break;
        }
    }
    if (keyboard != KEYBOARD_ANY) {
        if (res.size() > 0) res.append("-");
        switch (keyboard) {
            case ResTable_config::KEYBOARD_NOKEYS:
                res.append("nokeys");
                break;
            case ResTable_config::KEYBOARD_QWERTY:
                res.append("qwerty");
                break;
            case ResTable_config::KEYBOARD_12KEY:
                res.append("12key");
                break;
            default:
                res.append("keyboard=").append(dtohs(keyboard));
                break;
        }
    }
    if ((inputFlags&MASK_KEYSHIDDEN) != 0) {
        if (res.size() > 0) res.append("-");
        switch (inputFlags&MASK_KEYSHIDDEN) {
            case ResTable_config::KEYSHIDDEN_NO:
                res.append("keysexposed");
                break;
            case ResTable_config::KEYSHIDDEN_YES:
                res.append("keyshidden");
                break;
            case ResTable_config::KEYSHIDDEN_SOFT:
                res.append("keyssoft");
                break;
        }
    }
    if (navigation != NAVIGATION_ANY) {
        if (res.size() > 0) res.append("-");
        switch (navigation) {
            case ResTable_config::NAVIGATION_NONAV:
                res.append("nonav");
                break;
            case ResTable_config::NAVIGATION_DPAD:
                res.append("dpad");
                break;
            case ResTable_config::NAVIGATION_TRACKBALL:
                res.append("trackball");
                break;
            case ResTable_config::NAVIGATION_WHEEL:
                res.append("wheel");
                break;
            default:
                res.append("navigation=").append(dtohs(navigation));
                break;
        }
    }
    if ((inputFlags&MASK_NAVHIDDEN) != 0) {
        if (res.size() > 0) res.append("-");
        switch (inputFlags&MASK_NAVHIDDEN) {
            case ResTable_config::NAVHIDDEN_NO:
                res.append("navsexposed");
                break;
            case ResTable_config::NAVHIDDEN_YES:
                res.append("navhidden");
                break;
            default:
                res.append("inputFlagsNavHidden=")
                        .append(dtohs(inputFlags&MASK_NAVHIDDEN));
                break;
        }
    }
    if (screenSize != 0) {
        if (res.size() > 0) res.append("-");
        res.append(dtohs(screenWidth)).append("x").append(dtohs(screenHeight));
    }
    if (version != 0) {
        if (res.size() > 0) res.append("-");
        res.append("v").append(dtohs(sdkVersion));
        if (minorVersion != 0) {
            res.append(".").append(dtohs(minorVersion));
        }
    }

    return res.toStdString();
}

