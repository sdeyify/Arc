const { Object } = import "Object.class.arc";

public interface Iterable(
    type T;
) extends Object {
    public fn concat(ref const iterable: Iterable) -> typeof this;
    public fn concatToSelf(ref const iterable: Iterable) -> unit;
}