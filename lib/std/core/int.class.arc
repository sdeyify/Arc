const { String } = import circref<class int> "String.class.arc";
const { bool } = import circref<class int> "bool.class.arc";
const { ptr } = import circref<class int> "ptr.class.arc";
const { shared_ptr } = import circref<class int> "shared_ptr.class.arc";
const { unique_ptr } = import circref<class int> "unique_ptr.class.arc";
const { weak_ptr } = import circref<class int> "weak_ptr.class.arc";
const { owned_ptr } = import circref<class int> "owned_ptr.class.arc";

native public class int {
    native public fn stringify() -> String;
    native public fn id() -> int;
    native public fn equals(ref const obj: anyobj) -> bool;
    native public fn deepcopy() -> typeof this;
    native public fn shallow_copy() -> typeof this;
    native public fn ptr() -> ptr<typeof this>;
    native public fn shared_ptr() -> shared_ptr<typeof this>;
    native public fn unique_ptr() -> unique_ptr<typeof this>;
    native public fn weak_ptr() -> weak_ptr<typeof this>;
    native public fn owned_ptr() -> owned_ptr<typeof this>;
}