

public class HelloWorld { 

    public static void main(String[] args) throws InterruptedException {

	MyString myString = new MyString();
	System.out.println(myString.getHelloWorld());
	Thread.sleep(10000);
	

    }

    public static class MyString {

	public String getHelloWorld(){

	    String helloWorldString = "Hello, World";

	    return helloWorldString;

	}
	 
    }

}
