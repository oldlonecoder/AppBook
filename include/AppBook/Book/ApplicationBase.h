
#include <BookApplicationSkel/Interface.h>
#include <AppBook/Util/Object.h>
#include <AppBook/Console/Console.h>
#include <AppBook/Util/CArgs.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
namespace Book
{

class APPBOOK_EXPORTS ApplicationBase : public Util::Object
{

    static void InstallSignals();
    ApplicationBase();
    ConIO::Console Con{};

protected:
    virtual Result Setup(); ///< Call first from the derived::Setup(), then continue the Setting up...
    StrAcc::CArray mArgs;
    Cmd::CArgs Args{};
public:
    ApplicationBase(const std::string& AppName, int argc=0, char** argv=nullptr);
    ~ApplicationBase() override;

    virtual Result Run()=0;
    static ApplicationBase& Instance();
    ConIO::Console& Console();
    Book::Result ProcessArguments();
};

} // Book

//#endif //APPBOOK_APPLICATION_H

#pragma clang diagnostic pop