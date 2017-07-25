#ifndef __ENABLE_IF__H
#define __ENABLE_IF__H

namespace Grok3d {
    namespace Templates {

        //this is a general template that will be used
        template<bool B, class T = void>
        struct enable_if {};

        //this is a template specializaiton that will be used if the first template param is true
        template<class T>
        struct enable_if<true, T>
        {
            typedef T type;
        };
    } /*Templates*/
} /*Grok3d*/

#endif
