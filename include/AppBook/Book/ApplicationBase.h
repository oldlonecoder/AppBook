
#include <BookApplicationSkel/Interface.h>
#include <AppBook/Util/Object.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
namespace Book
{

class APPBOOK_EXPORTS ApplicationBase : public Util::Object
{

    static void InstallSignals();
    ApplicationBase();

    StrAcc::CArray mArgs;
protected:
    virtual Result Setup(); ///< Call first from the derived::Setup(), then continue the Setting up...

public:
    ApplicationBase(const std::string& AppName, int argc=0, char** argv=nullptr);
    ~ApplicationBase() override;

    virtual Result Run()=0;

};

} // Book

//#endif //APPBOOK_APPLICATION_H

#pragma clang diagnostic pop