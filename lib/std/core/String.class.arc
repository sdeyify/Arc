const { bool } = import circref<class String> "bool.class.arc";
const { int } = import circref<class String> "int.class.arc";
const { ptr } = import circref<class String> "ptr.class.arc";
const { shared_ptr } = import circref<class String> "shared_ptr.class.arc";
const { unique_ptr } = import circref<class String> "unique_ptr.class.arc";
const { weak_ptr } = import circref<class String> "weak_ptr.class.arc";
const { owned_ptr } = import circref<class String> "owned_ptr.class.arc";

native public class String {
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