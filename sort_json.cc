#include <cstdio>
#include <string>

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include <sstream>
#include <algorithm>
#include "picojson.h"
using namespace std;

bool sortByValues (picojson::value i, picojson::value j) {
  if(i.is<picojson::object>()) {
    if(j.is<picojson::object>()) {
      return (i.get<picojson::object>().begin()->second.get<double>() < j.get<picojson::object>().begin()->second.get<double>());
    } else {
      return false;
    };
  } else {
    return false;
  };
};
class HelloTutorialInstance : public pp::Instance {
public:
  /// The constructor creates the plugin-side instance.
  /// @param[in] instance the handle to the browser-side plugin instance.
  explicit HelloTutorialInstance(PP_Instance instance) : pp::Instance(instance)
  {}
  virtual ~HelloTutorialInstance() {}

  /// Handler for messages coming in from the browser via postMessage().  The
  /// @a var_message can contain anything: a JSON string; a string that encodes
  /// method names and arguments; etc.  For example, you could use
  /// JSON.stringify in the browser to create a message that contains a method
  /// name and some parameters, something like this:
  ///   var json_message = JSON.stringify({ "myMethod" : "3.14159" });
  ///   nacl_module.postMessage(json_message);
  /// On receipt of this message in @a var_message, you could parse the JSON to
  /// retrieve the method name, match it to a function call, and then call it
  /// with the parameter.
  /// @param[in] var_message The message posted by the browser.
  virtual void HandleMessage(const pp::Var& var_message) {

         picojson::value v;
         const string json1(var_message.AsString());

         istringstream iss2(json1);
         string err = picojson::parse(v, iss2);

         if(v.is<picojson::array>()) {
            picojson::array v_array(v.get<picojson::array>());
            std::sort(v_array.begin(), v_array.end(), sortByValues);
            picojson::value v1(v_array);
            std::string v_str(v1.serialize());
            PostMessage( v_str.c_str() );
         } else {
            PostMessage("Error! Not an array!");
         };

  };
};

/// The Module class.  The browser calls the CreateInstance() method to create
/// an instance of your NaCl module on the web page.  The browser creates a new
/// instance for each <embed> tag with type="application/x-nacl".
class HelloTutorialModule : public pp::Module {
public:
  HelloTutorialModule() : pp::Module() {}
  virtual ~HelloTutorialModule() {}

  /// Create and return a HelloTutorialInstance object.
  /// @param[in] instance The browser-side instance.
  /// @return the plugin-side instance.
  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new HelloTutorialInstance(instance);
  }
};

namespace pp {
  Module* CreateModule() {
    return new HelloTutorialModule();
  }
}  // namespace pp

