//
// c++ test.cpp -o test -std=c++0x `pkg-config --libs --cflags icu-uc icu-io` -I/usr/include/x86_64-linux-gnu  -licuio -licui18n -licuuc -licudata 
//

#include <stdio.h>
#include "unicode/unistr.h"
#include "unicode/utypes.h"
#include "unicode/locid.h"
#include "unicode/coll.h"
#include "unicode/tblcoll.h"
#include "unicode/coleitr.h"
#include "unicode/sortkey.h"
using namespace icu;
UBool collateWithLocaleInCPP(const Locale& locale, UErrorCode& status)
{
    UnicodeString dispName; 
    UnicodeString source("This is a test.");
    UnicodeString target("THIS IS A TEST.");
    Collator::EComparisonResult result    = Collator::EQUAL;
    CollationKey sourceKey;
    CollationKey targetKey; 
    Collator      *myCollator = 0;
    if (U_FAILURE(status))
    {
        return false;
    }
    myCollator = Collator::createInstance(locale, status);
    if (U_FAILURE(status)){
        locale.getDisplayName(dispName);
        /*Report the error with display name... */
        // fprintf(stderr,
        // "%s: Failed to create the collator for : \"%s\"\n", dispName);
        return false;
    }
    result = myCollator->compare(source, target);
    /* result is 1, secondary differences only for ignorable space characters*/
    if (result != UCOL_LESS)
    {
        fprintf(stderr,
        "Comparing two strings with only secondary differences in C failed.\n");
        return false;
    }
    /* To compare them with just primary differences */
    myCollator->setStrength(Collator::PRIMARY);
    result = myCollator->compare(source, target);
    /* result is 0 */
    if (result != 0)
    {
        fprintf(stderr,
        "Comparing two strings with no differences in C failed.\n");
        return false;
    }
    /* Now, do the same comparison with keys */
    myCollator->getCollationKey(source, sourceKey, status);
    myCollator->getCollationKey(target, targetKey, status);
    result = Collator::EQUAL;

    result = sourceKey.compareTo(targetKey);
    if (result != 0)
    {
        fprintf(stderr,
        "%s: Comparing two strings with sort keys in C failed.\n");
        return false;
    }
    delete myCollator;
    return true;
}

// extern "C" UBool collateWithLocaleInC(const char* locale, UErrorCode *status);
int main()
{
   UErrorCode status = U_ZERO_ERROR;
   fprintf(stdout, "\n");
   if (collateWithLocaleInCPP(Locale("en", "US"), status) != true)
   {
        fprintf(stderr,
        "Collate with locale in C++ failed.\n");
   } else 
   {
       fprintf(stdout, "Collate with Locale C++ example worked!!\n");
   }
   return 0;
}
