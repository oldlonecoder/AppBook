
#include <AppBook/Book/AppBook.h>
#include <AppBook/Util/Object.h>
#include <AppBook/Util/CArgs.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
namespace Book
{

class APPBOOK_API ApplicationBase : public Util::Object
{

    static void InstallSignals();
    ApplicationBase();


protected:
    virtual Result Setup(); ///< Call first from the derived::Setup(), then continue the Setting up...
    StrAcc::CArray mArgs;
    Cmd::CArgs Args{};
public:
    ApplicationBase(const std::string& AppName, int argc=0, char** argv=nullptr);
    ~ApplicationBase() override;

    virtual Result Run()=0;
    static ApplicationBase& Instance();
    Book::Result CompileArguments();
};

} // Book

//#endif //APPBOOK_APPLICATION_H

#pragma clang diagnostic pop