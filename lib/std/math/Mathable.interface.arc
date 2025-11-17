functor binaryMathFn = functor<T>(other: Mathable) -> T;

transparent interface Mathable {
    type T;

    public native fn add<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn sub<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn mul<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn div<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn mod<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn pow<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn root<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn square_root<T = this.T>() -> T { return this.root(2); }
    public native fn cube_root<T = this.T>() -> T { return this.root(3); }
    public native fn greater_than<T = this.T>(other: Mathable) satisfies binaryMathFn -> bool;
    public native fn less_than<T = this.T>(other: Mathable) satisfies binaryMathFn -> bool;
    public native fn greater_or_equal_to<T = this.T>(other: Mathable) satisfies binaryMathFn -> bool;
    public native fn lesser_or_equal_to<T = this.T>(other: Mathable) satisfies binaryMathFn -> bool;

    public native fn sin<T = this.T>() -> T;
    public native fn cos<T = this.T>() -> T;
    public native fn tan<T = this.T>() -> T;
    public native fn cot<T = this.T>() -> T;
    public native fn sec<T = this.T>() -> T;
    public native fn csc<T = this.T>() -> T;
    public native fn asin<T = this.T>() -> T;
    public native fn acos<T = this.T>() -> T;
    public native fn atan<T = this.T>() -> T;

    public native fn logb<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn ln<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn log10<T = this.T>() -> T { return this.logb(10); }
    public native fn exp<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;

    public native fn max<T = this.T>(other: Mathable) satisfies binaryMathFn -> T {
        return if (this.greater_than(other)) this else other
    }

    public native fn min<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn abs<T = this.T>() -> T;
    public native fn floor<T = this.T>() -> T;
    public native fn ceil<T = this.T>() -> T;

    public native fn sign<T = this.T>() -> -1 | 0 | 1 {
        return if (this.greater_than(0)) 1 else if (this.less_than(0)) -1 else 0;
    }

    public native fn left_shift<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn right_shift<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn and<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn or<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn xor<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
    public native fn not<T = this.T>(other: Mathable) satisfies binaryMathFn -> T;
}