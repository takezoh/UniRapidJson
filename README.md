# UniRapidJSON

UniRapidJSON is [RapidJSON][] wrapper for Unity.

UniRapidJSON does not extend mono's heap memory.   
Because [RapidJSON][] uses native memory to parse JSON strings.

Supported Platforms: iOS, Android, Windows, OSX  
Experimental Supported: WinRT

UniRapidJSON currently only supports JSON Parser and JOIN Pointer.  
Note that, Writable functions are not supported.

# Tutorial

UniRapidJSON implements the [RapidJSON][] like interface.  
Let's challenge [Tutorial of RapidJSON](http://rapidjson.org/md_doc_tutorial.html) with UniRapidJSON.

## Query Value

There is the following JSON string stored in `string`:

```
{
    "hello": "world",
    "t": true ,
    "f": false,
    "n": null,
    "i": 123,
    "pi": 3.1416,
    "a": [1, 2, 3, 4]
}
```

Parse it into a Document:

```csharp
using rapidjson;

// ...
using (var document = Document.Parse(json)) {
    // rapidjson.Document inherits the System.IDisposable interface.
}
```

Let's query whether a `"hello"` member exists in the root object. Since a `Value` can contain different types of value, we may need to verify its type and use suitable API to obtain the value. In this example, `"hello"` member associates with a JSON string.

```csharp
Assert.IsTrue(document.Root.HasMember("hello"));
Assert.IsTrue(document.Root["hello"].IsString());
Debug.Log(string.Format("hello = {0}", document.Root["hello"].ToString()));
```
```
hello = world
```

JSON true/false values are represented as `bool`.

```csharp
Assert.IsTrue(document.Root["t"].IsBool());
Debug.Log(string.Format("t = {0}", document.Root["t"].ToBool() ? "true" : "false"));
```
```
t = true
```

JSON null can be queried with `IsNull()`.

```csharp
Debug.Log(string.Format("n = {0}", document.Root["n"].IsNull() ? "null" : "?"));
```
```
n = null
```

JSON number type represents all numeric values. However, C# needs more specific type for manipulation.

```csharp
Assert.IsTrue(document.Root["i"].IsNumber());

// In this case, IsUint()/IsLong()/IsULong() also return true.
Assert.IsTrue(document.Root["i"].IsInt());          
Debug.Log(string.Format("i = {0}", document.Root["i"].ToInt()));
// Alternative (int)document.Root["i"]

Assert.IsTrue(document.Root["pi"].IsNumber());
Assert.IsTrue(document.Root["pi"].IsDouble());
Debug.Log(string.Format("pi = {0}", document.Root["pi"].ToDouble()));
```
```
i = 123
pi = 3.1416
```

JSON array contains a number of elements.

```csharp
var a = document.Root["a"];
Assert.IsTrue(a.IsArray());
for (int i = 0; i < a.Size(); i++) 
    Debug.Log(string.Format("a[{0}] = {1}", i, a[i].ToInt()));
```
```
a[0] = 1
a[1] = 2
a[2] = 3
a[3] = 4
```

Note that, RapidJSON does not automatically convert values between JSON types. If a value is a string, it is invalid to call `ToInt()`, for example. In debug mode it will fail an assertion. In release mode, the behavior is undefined.

In the following sections we discuss details about querying individual types.

## Query Array

You may access the elements in an array by integer literal, for example, a[0], a[1], a[2].  
`Array` is inherits `IEnumerable`: instead of using indices, you may also use foreach statement to access all the elements.

```csharp
foreach (var value in a.GetArray()) 
    Debug.Log(string.Format("{0}", value.ToInt()));
```

## Query Object

Similar to Array, we can access all object members by foreach statement:  

```csharp
foreach (var member in document.Root.GetObject())
    Debug.Log(string.Format("Type of member {0} is {1}",
        member.Key, member.Value.GetValueType()));
```

Note that, when `this[string]: indexer` cannot find the member, it will throw an `KeyNotFoundException`.  

If we are unsure whether a member exists, we need to call `HasMember()` before calling `this[string]: indexer`. However, this incurs two lookup. A better way is to call `TryGetValue()`, which can check the existence of member and obtain its value at once:

```csharp
var root = document.Root.GetObject();
Value value;
if (root.TryGetValue("hello", out value))
    Debug.Log(value.ToString());
```

## JSON Pointer

A JSON Pointer is a list of zero-to-many tokens, each prefixed by `/`. Each token can be a string or a number. For example, given a JSON:

```
{
    "foo" : ["bar", "baz"],
    "pi" : 3.1416
}
```

The following JSON Pointers resolve this JSON as:

+ `"/foo"` → `[ "bar", "baz" ]`
+ `"/foo/0"` →` "bar"`
+ `"/foo/1"` →` "baz"`
+ `"/pi"` →` 3.1416`

Note that, an empty JSON Pointer `""` (zero token) resolves to the whole JSON.

## Usage JSON Pointer

```csharp
Value a;
if (document.Root.TryGetValueByPointer("/foo", out a)) 
    foreach (var v in a.GetArray()) 
        Debug.Log(v.ToString());

Debug.Log(document.Root.GetValueByPointer("/foo/0", "default string"));
Debug.Log(document.Root.GetValueByPointer("/foo/1", "default string"));
Debug.Log("" + document.Root.GetValueByPointer("/pi", 0.0));
```
```
bar
baz
bar
baz
3.1416
```


# LICENSE

UniRapidJSON is released under the MIT license, see LICENSE.  

Thanks for RapidJSON contributors.

[RapidJSON]:http://rapidjson.org/
