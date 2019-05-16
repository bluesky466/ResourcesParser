# ResourcesParser

android resources.arsc parser

```
rp -p path [-a] [-t type] [-i id]

-p : set path of resources.arsc
-a : show all of resources.arsc
-t : select the type in resources.arsc to show
-i : select the id of resource to show
```

## Example:

show all resource:

> ./rp -p /Users/linjw/workspace/MyApplication5/app/build/outputs/apk/debug/app-debug/resources.arsc -a

result:

[all.txt](https://raw.githubusercontent.com/bluesky466/ResourcesParser/master/result/all.txt)

show string resource:

> ./rp -p /Users/linjw/workspace/MyApplication5/app/build/outputs/apk/debug/app-debug/resources.arsc -t string

result:

[string.txt](https://raw.githubusercontent.com/bluesky466/ResourcesParser/master/result/string.txt)

show designative resource (id 0x7f0b0016):

> ./rp -p /Users/linjw/workspace/MyApplication5/app/build/outputs/apk/debug/app-debug/resources.arsc -i 0x7f0b0016

result:

```
string : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ca : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Supr

string-da : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-fa : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ja : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-ka : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) წაშლა

string-pa : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ta : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) டெலிட்

string-nb : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-be : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-de : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Löschen

string-ne : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-te : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-af : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-bg : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-th : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-fi : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-hi : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-si : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) මකන්න

string-vi : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-kk : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-mk : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) избриши

string-sk : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) odstrániť

string-uk : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-el : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-gl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) eliminar

string-ml : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) ഇല്ലാതാക്കുക

string-nl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) verwijderen

string-pl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-sl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-tl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-am : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) ሰርዝ

string-km : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-bn : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) মুছুন

string-in : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-kn : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-mn : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) устгах

string-ko : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) 삭제

string-lo : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ro : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-sq : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ar : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-fr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) supprimer

string-hr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-mr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) हटवा

string-or : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) ଡିଲିଟ୍‍

string-sr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-sr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-tr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) sil

string-ur : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-as : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) মচক

string-bs : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-cs : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-es : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Eliminar

string-is : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ms : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) padam

string-et : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) kustutamisklahv

string-it : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) CANC

string-lt : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) „Delete“

string-pt : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-eu : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Ezabatu

string-gu : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) ડિલીટ કરો

string-hu : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-ru : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-zu : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) susa

string-lv : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) dzēšanas taustiņš

string-sv : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-iw : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-sw : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) futa

string-hy : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-ky : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-my : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-az : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) silin

string-uz : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-en-CA : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-fr-CA : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) supprimer

string-en-GB : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-en-XC : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) 8‎‏‎‎‎‎‎‏‎‏‏‏‎‎‎‎‎‏‎‎‏‎‎‎‎‏‏‏‏‏‏‏‏‏‏‎‏‎‎‎‎‎‎‏‏‎‎‏‎‎‎‎‏‏‎‎‎‎‏‏‏‎‎‎‏‎‎‏‏‎‎‏‏‏‏‏‏‎‎‏‏‎‏‏‎‎‎‎‎‎‎‎‏‏‎‎delete‎‏‎‎‏‎

string-zh-HK : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) 刪除

string-zh-CN : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete 键

string-en-IN : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-pt-BR : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-es-US : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) borrar

string-pt-PT : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) eliminar

string-en-AU : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-zh-TW : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete 鍵

```

show designative resource (id 2131427350):

> ./rp -p /Users/linjw/workspace/MyApplication5/app/build/outputs/apk/debug/app-debug/resources.arsc -i 2131427350

result:

```
string : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ca : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Supr

string-da : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-fa : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ja : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-ka : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) წაშლა

string-pa : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ta : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) டெலிட்

string-nb : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-be : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-de : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Löschen

string-ne : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-te : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-af : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-bg : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-th : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-fi : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-hi : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-si : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) මකන්න

string-vi : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-kk : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-mk : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) избриши

string-sk : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) odstrániť

string-uk : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-el : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-gl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) eliminar

string-ml : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) ഇല്ലാതാക്കുക

string-nl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) verwijderen

string-pl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-sl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-tl : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-am : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) ሰርዝ

string-km : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-bn : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) মুছুন

string-in : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-kn : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-mn : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) устгах

string-ko : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) 삭제

string-lo : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ro : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-sq : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ar : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-fr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) supprimer

string-hr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-mr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) हटवा

string-or : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) ଡିଲିଟ୍‍

string-sr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-sr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-tr : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) sil

string-ur : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-as : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) মচক

string-bs : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-cs : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-es : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Eliminar

string-is : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-ms : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) padam

string-et : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) kustutamisklahv

string-it : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) CANC

string-lt : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) „Delete“

string-pt : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-eu : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Ezabatu

string-gu : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) ડિલીટ કરો

string-hu : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-ru : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-zu : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) susa

string-lv : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) dzēšanas taustiņš

string-sv : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-iw : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-sw : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) futa

string-hy : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-ky : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-my : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-az : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) silin

string-uz : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete

string-en-CA : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-fr-CA : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) supprimer

string-en-GB : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-en-XC : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) 8‎‏‎‎‎‎‎‏‎‏‏‏‎‎‎‎‎‏‎‎‏‎‎‎‎‏‏‏‏‏‏‏‏‏‏‎‏‎‎‎‎‎‎‏‏‎‎‏‎‎‎‎‏‏‎‎‎‎‏‏‏‎‎‎‏‎‎‏‏‎‎‏‏‏‏‏‏‎‎‏‏‎‏‏‎‎‎‎‎‎‎‎‏‏‎‎delete‎‏‎‎‏‎

string-zh-HK : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) 刪除

string-zh-CN : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete 键

string-en-IN : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-pt-BR : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-es-US : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) borrar

string-pt-PT : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) eliminar

string-en-AU : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) delete

string-zh-TW : 	abc_menu_delete_shortcut_label (2131427350 or 0x7f0b0016) = (string) Delete 鍵

```
