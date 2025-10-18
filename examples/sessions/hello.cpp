#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/http_response.h>
#include <cppcms/service.h>
#include <cppcms/session_interface.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <print>

#include <iostream>

#include "content.h"
#include "rules_cc/cc/runfiles/runfiles.h"

#include <unicode/brkiter.h>

using namespace std;
using rules_cc::cc::runfiles::Runfiles;

class hello : public cppcms::application {
 public:
  hello(cppcms::service &s) : cppcms::application{s} {
    std::println("{} app.root: {}", "constructed",
            settings().get<std::string>("app.root"));
    dispatcher().assign("/number/(\\d+)",&hello::number,this,1);
    mapper().assign("number","/number/{1}");
    dispatcher().assign("/mainRaw",&hello::mainRaw,this);
    mapper().assign("mainRaw","/mainRaw");
    dispatcher().assign("/",&hello::root,this);
    mapper().assign("/");
    mapper().root("/hello");
    // mapper().root(settings().get<std::string>("app.root"));
  }
  void number(std::string num)
  {
    std::println("{}", " in the number");
    int no = atoi(num.c_str());
    response().out() << "The number is " << no << "<br/>\n";
    response().out() << "<a href='" << url("/") << "'>Go back</a>";
  }
  void mainRaw() {
    response().out() << "<html>\n"
                        "<body>\n"
                        "  <h1>Hello ----- World</h1>\n"
                        "</body>\n"
                        "</html>\n";
  }
  virtual void root();
};


void hello::root() {
    std::println("{}", " inside the main");
    content::message c;
    // c.app(this);
    if (request().request_method() == "POST") {
        c.info.load(context());
        if (c.info.validate()) {
        session()["name"] = c.info.name.value();
        session()["sex"] = c.info.sex.selected_id();
        session()["state"] = c.info.martial.selected_id();
        session().set("age", c.info.age.value());
        c.info.clear();
        }
    }
    if (session().is_set("name")) {
        c.name = session()["name"];
        if (session()["sex"] == "m") {
        c.who = "Mr";
        } else {
        if (session()["state"] == "s") {
            c.who = "Miss";
        } else {
            c.who = "Mrs";
        }
        }
        c.age = session().get<double>("age");
    } else {
        c.name = "Visitor";
        c.age = -1;
    }
    render("message", c);
}

int main(int argc, char **argv) {
  std::unique_ptr<Runfiles> runfiles(Runfiles::Create(argv[0]));
  std::string path = runfiles->Rlocation("icu_dat/source/data/in/");
  u_setDataDirectory(path.c_str());

  std::println("{}", " in the main");
  try {
    cppcms::service srv(argc, argv);
    std::println("{}", " service is ready");
    srv.applications_pool().mount(cppcms::applications_factory<hello>());
    std::println("{}", "pool mounted");
    srv.run();
    std::println("{}", "finished running");
  } catch (std::exception const &e) {
    cerr << e.what() << endl;
  }
}