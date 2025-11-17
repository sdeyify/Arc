const { Object } = import "Object.class.arc";
const { bool } = import "Object.class.arc";

native public class Symbol(
    private type T;
    public key: T;
    private unique: bool = true;
) extends Object {
    public fn getKey() -> T {
        return this.key;
    }
}