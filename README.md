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

show string resource:

> ./rp -p /Users/linjw/workspace/MyApplication5/app/build/outputs/apk/debug/app-debug/resources.arsc -t string

show designative resource (id 0x7f0b0016):

> ./rp -p /Users/linjw/workspace/MyApplication5/app/build/outputs/apk/debug/app-debug/resources.arsc -i 0x7f0b0016

show designative resource (id 2131427350):

> ./rp -p /Users/linjw/workspace/MyApplication5/app/build/outputs/apk/debug/app-debug/resources.arsc -i 2131427350