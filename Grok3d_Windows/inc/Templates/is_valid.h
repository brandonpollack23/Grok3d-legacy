#ifndef __IS_VALID__H
#define __IS_VALID__H

namespace Grok3d {
    namespace Templates {

        template <typename LambdaTemplate>
        struct IsValidContainer
        {
        private:
            //the return type we are trying to match here can be describe as such:
            // match if there is a type such that when the struct this Lambda represent's () templated function
            // calls on the object passed as Param, the return type declared in the lambda exists
            //eg, auto hasSerialize = is_valid([](auto&& x) -> decltype(x.serialize()) { }); there must be a function x.serialize() in order for decltype to return a type
            // so if the object passed to this lambda, and therefore the template function for () on the struct who represents it, does not have such a member, this template will fail and the black hole will be made instead
            // then if this matches, we just need to use the decltype that () returned to get the true_type or false_type with the internal constexpr value that is false or true
            //int is only used for precedence over the black hole's ...
            template <typename Param> 
            constexpr auto testValidity(int /* unused */)
            -> decltype(std::declval<LambdaTemplate>()(std::declval<Param>()), std::true_type())
            {
                // If substitution didn't fail, we can return a true_type.
                return std::true_type();
            }

            template <typename Param> constexpr std::false_type testValidity(...)
            {
                // Our sink-hole returns a false_type.
                return std::false_type();
            }

        public:
            //return type is automatic because it is determined later by the templated () function within the captured LambdaTemplate
            template <typename Param> constexpr auto operator()(const Param& p)
            {
                return testValidity<Param>(int());
            }
        };

        template <typename LambdaTemplate> constexpr auto is_valid(const LambdaTemplate& t)
        {
            // We used auto for the return type: it will be deduced here.
            return IsValidContainer<LambdaTemplate>();
        }

        // example, is_valid creates a method
        //auto hasSerialize = is_valid([](auto&& x) -> decltype(x.serialize()) { });
    } /*Templates*/
} /*Grok3d*/

#endif