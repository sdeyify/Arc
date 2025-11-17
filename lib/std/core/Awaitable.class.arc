const { String } = import "String.class.arc";
const { bool } = import "bool.class.arc";
const { int } = import "int.class.arc";
const { ptr } = import "ptr.class.arc";
const { shared_ptr } = import "shared_ptr.class.arc";
const { unique_ptr } = import "unique_ptr.class.arc";
const { weak_ptr } = import "weak_ptr.class.arc";
const { owned_ptr } = import "owned_ptr.class.arc";

native public class Awaitable(
    private type T
) {
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

    native public fn await() -> T;
}

type Future<T> = Awaitable<Awaitable<T>>;