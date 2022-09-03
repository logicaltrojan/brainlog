
## Parent / Child Class 간 Casting 

서로 상속관계에 있는 클래스 타입의 참조변수간의 형변환은 양방향으로 자유롭게 수행될 수 있으나, 참조변수가 참조하고 있는 인스턴스의 자손타입으로 형변환을 하는 것은 허용되지 않는다. 

```java

   Car car = new Car();
        Car car2 = new Car();
        FireTruck fireTruck = null;
        FireTruck fireTruck1 = new FireTruck();
        
        Car car3 = new FireTruck(); // ok


        // fireTruck  = (FireTruck) car;
//        Exception in thread "main" java.lang.ClassCastException: com.company2.Main$Car cannot be cast to com.company2.Main$FireTruck
//        at com.company2.Main.main(Main.java:20)

        car2 = (Car) fireTruck1;


```

## Parent / Child Cast 후 변수/ method 접근

```java

   public static void main(String[] args) throws IOException {

        Parent p = new Child();
        Child c = new Child();

        System.out.println(p.x); //100
        System.out.println(c.x); //200
        p.method(); //This is Child
        c.method(); //This is Child

    }


    static class Parent {

        int x = 100;
        void method() {
            System.out.println("This is parent");
        }

    }

    static class Child extends Parent {

        int x = 200;
        void method() {
            System.out.println("This is child");
        }

    }


```

method : 항상 실제 인스턴스의 메서드 (casting되어도 new keyword로 만들어진 method가 호출) 
member : 참조변수의 타입에 따라 결정됨 



```java

    public static void main(String[] args) throws IOException {

        class LocalInner {
//            static int i = 300;
            final static int CONST = 300;
        }

    }

    class InstanceInner {
        int i = 100;
//        static int cv = 100;

        final static int a = 100;

    }

    static class StaticInner {
        static int i = 100;
    }

```

| Type | Static | Final Static | Local|
|-----| -------|--------------|------|
|Instance Inner | X | O | O
|Static Inner | O | O | O
|Local Inner  | X | O | O




## Outer Class Member Access to Inner Class

```java

public class Main {

    private int PRIVATE = 3;
    private final int FINAL_PRIVATE = 4;
    static int STATIC = 4;
    private static int STATIC2 = 4;

    public void method() {

        class LocalInner {
            final static int CONST = 300; 
            int a = STATIC;
            int b = STATIC2;
            int c = PRIVATE;
            int v = FINAL_PRIVATE; 
            StaticInner t = new StaticInner();
            InstanceInner k = new InstanceInner();

        }

    }

    class InstanceInner {
        int i = 100;
//        static int cv = 100;

        int a = STATIC;
        int x = PRIVATE;
        final static int k = 100;

    }

    static class StaticInner {
        static int i = 100;
        int a = new Main().PRIVATE;
        int k = STATIC;
//        int v = PRIVATE;
    }


}

```


| Type | Outer Instance| Outer Static | Private| InstanceInner | StaticInner
|-----| -------|--------------|------| ---| -- |
|Instance Inner | O | O | O | O | O
|Static Inner | X | O | O | X | O
|Local Inner  | O | O | O | O | O




## Exception 

try-catch-finally

exception occured : try -> catch -> finally
exception not occured : try -> finally 


try 에 return 문이 있더라도 ,finally문이 실행된 후에 return 된다.
심지어 try, catch 문에 return 이 있더라도 finally가 실행된 이후에 실행된다. 


below method returns 2
```java

    public static int method(){

        try {
            System.out.println("try");
            return 0;
            throw new Exception();
        }catch(Exception e){

            System.out.println("catch ");
            return 1;

        }finally {

            System.out.println("finally");
            return 2;

        }


    }

```

below method returns 0

```java

    public static int method(){

        try {
            System.out.println("try");
            return 0;
            throw new Exception();
        }catch(Exception e){

            System.out.println("catch ");
            return 1;

        }finally {

            System.out.println("finally");

        }


    }

```

return이 있는 method면, catch문에 return이 있거나, finally에 return 이 정의되어 있어야 한다. 



## Clone


Cloneable interface 를 구현한 경우에는 shallow / deep copy를 구분해야함

기본적으로 primitive type의 경우는 Object . clone()을 사용해도 상관없지만 Reference type의 경우 같은 객체를 가르키기 때문에 deep copy를 구현하기 위해서는 객체의 deep copy를 구현해야함


int[] arr의 경우 객체 타입의 reference를 그대로 복사해오기 때문에 shallow copy밖에 안됨 

```java

public class Main {


    public static class Point implements Cloneable {

        private int x;
        private int y;
        private int[] arr = { 1, 2, 3};

        public void setArrEle(int idx, int val){
            this.arr[idx] = val;

        }

        public void setX(int x) {
            this.x = x;
        }

        public void setY(int y) {
            this.y = y;
        }

        public int getX() {
            return x;
        }

        public int getY() {
            return y;
        }

        public Point(int x, int y) {
            this.x = x;
            this.y = y;
        }


        private void printArr(){
            Arrays.stream(this.arr).forEach(System.out::print);

        }

        @Override
        public String toString(){

            System.out.println("Arr : " );
            this.printArr();;
            return "x : " + x + "  y : " + y;
        }


        @Override
        public Point clone() {
            try {
                Point clone = (Point) super.clone();
                return clone;
            } catch (CloneNotSupportedException e) {
                throw new AssertionError();
            }
        }
    }




    public static void main(String[] args) throws IOException {
        Point original = new Point(1, 2);
        Point copy = original.clone();

        System.out.println(original);
        System.out.println(copy);

        copy.setX(9);
        copy.setY(9);
        copy.setArrEle(0, 999);

        System.out.println(original); // 999 , 2, 3
        System.out.println(copy); // 999, 2, 3

    }


```

to make a deep copy,  a
```java

   @Override
        public Point clone() {
            try {
                Point clone = (Point) super.clone();
                clone.setArr(this.arr.clone());
                return clone;
            } catch (CloneNotSupportedException e) {
                throw new AssertionError();
            }
        }

```


Generic / Generic Inheritance

```java


// Java Program to illustrate generic class hierarchies
 
// Importing all input output classes
import java.io.*;
 
// Helper class 1
// Class 1 - Parent class
class Generic1<T> {
    // Member variable of parent class
    T obj;
 
    // Constructor of parent class
    Generic1(T o1) { obj = o1; }
 
    // Member function of parent class
    // that returns an object
    T getobj1() { return obj; }
}
 
// Helper class 2
// Class 2 - Child class
class Generic2<T, V> extends Generic1<T> {
    // Member variable of child class
    V obj2;
    Generic2(T o1, V o2)
    {
        // Calling super class using super keyword
        super(o1);
 
        obj2 = o2;
    }
 
    // Member function of child class
    // that returns an object
    V getobj2() { return obj2; }
}
 
// Class 3 -  Main class
class GFG {
   
    // Main driver method
    public static void main(String[] args)
    {
        // Creating Generic2 (sub class) object
      // Custom inputs as parameters
        Generic2<String, Integer> x
            = new Generic2<String, Integer>("value : ",
                                            100);
 
        // Calling method and printing
        System.out.println(x.getobj1());
        System.out.println(x.getobj2());
    }
}

```