
왜 우리는 IOC container를 이용하여 DI pattern을 사용하는 걸까? 


확장되지 않으려고 설계된 Application은 없다.(일부러 그러는 것이 아니면..)
우리가 함수를 나누고, 클라스를 나누고, SOLID 원칙과 component 분리 등등을 열심히 머리 아파가며 수행하는 이유는, 지금은 작더라도 창대해질 수 있는 어플리케이션을 기본적으로 생각하기 때문이다. 


Terminology 

Component : 

Glob of software that's intended to be used, without change, by an application that is out of the control of the writers of the component.

컴퍼넌트 작성자가 제어할 수 없는 응용프로그램에서변경없이 사용하도록 의도된 소프트웨어 (집합) 
'변경 없이'란 컴퍼넌트 작성자가 허용하는 방식으로 확장하여 동작을 변경할 수있지만, 사용중인 응용프로그램이 컴퍼넌트의 소스코드를 변경하지 않는다. 


Service : 

서비스란 일반적으로 흔히 일컫는 응용 프로그램의 개념과 같다, 컴포넌트와 차이점이라면, 서비스는 원격 인터페이스를 통해 사용될 수도 있다는 점이다.(HTTP, gRPC)


```java
class MovieLister...

  public Movie[] moviesDirectedBy(String arg) {
      List allMovies = finder.findAll();
      for (Iterator it = allMovies.iterator(); it.hasNext();) {
          Movie movie = (Movie) it.next();
          if (!movie.getDirector().equals(arg)) it.remove();
      }
      return (Movie[]) allMovies.toArray(new Movie[allMovies.size()]);
  }

```

MovieLister가 하는 일 : 인자로 받은 감독 영화를 Return함 

finder가 하는 일 : findAll() -> 영화를 return함 

finder가 어떻게 영화를 저장하는지, (심지어 저장하기는 하는지.. 랜덤으로 주던지. . ) 상관 없이 moviesDirectedBy의 method를 사용하고 싶다.

```java
public interface MovieFinder { 
    List findAll();
}
```

```java 
public class ColonDelimitedMovieFinder implements MovieFinder { 
    public List findAll(){
    //help yourself
    }

}

private MovieFinder finder;
public MovieLister(){
    finder = new ColonDelimitedMovieFinder("movies.txt");
}

```

이렇게 사용하면 될까? 만약 이 method를 누가 가져다가 쓸 경우, movies.txt라는 파일이 resource에 없으면 어떻게 되는가? movies2.txt는 있다면... SQL , xml 이라면 ... 

https://www.martinfowler.com/articles/injection/naive.gif

MovieLister는 MovieFinder, MovieFinderImpl로 부터 자유로워졌지만, 이 instance끼리 서로 어떻게 엮을 것인가? 

Patterns of Enterprise Application Architecture에서 이런 상황을 plugin이라고 소개했다. 
implementation이 된 class(구현체)가 complie time에 연결된 것이 아닌, 나중에 어느 순간에 연결된다. 중요한 점은 , 나의 lister class가 구현체에 영향 받지 않으면서 이 연결을 어떻게 수행하고, 연결된 객체를 사용할 것인가이다. 


실제 개발에서도, 우리가 추상화한 class가 몇 십개가 존재할 때, 이를 선택하여 서비스로 어떻게 배포할 것인가의 문제라고 볼 수 있다. 


Inversion of Control

무엇을 inversion함? 

- framework라는 것이 자꾸 IOC 를 특별하게 말하는데, 이것은 자동차가 바퀴가 있다는 것처럼 조금 당연한 것들이다. 
- Framework에 IOC는, "we call you"라는 개념으로,  사용자가 특정 클래스를 implementation하거나, handler를 구현하면, 이를 framework가 사용한다는 개념이 있다. (Spring을 생각해보면, 우리는 Controller를 시작하지 않는다. 그 어떤것도 시작하지 않는다.)
- IoC는 너무 일반적인 term이기 때문에, Dependency Injection이라고 하는 것이 위의 implementation + plugin 문제를 정의하는 term이 맞는 것 같다. 


Forms of Dependency Injection


- 크게 세가지 방법이 존재
- Constructor Injection, Setter Injection, Interface inejction 


### Constructor Inejction with PicoContainer

- PicoContainer
- constructor로 inject하는 방식을 사용함
- MovieLister의 경우, 따라서 모든 injection이 필요한 class를 받는 constructor를 선언해줘야 함 

```java
class MovieLister...

  public MovieLister(MovieFinder finder) {
      this.finder = finder;       
  }
```
```java
class ColonMovieFinder...

  public ColonMovieFinder(String filename) {
      this.filename = filename;
  }
```

```java
private MutablePicoContainer configureContainer() {
    MutablePicoContainer pico = new DefaultPicoContainer();
    Parameter[] finderParams =  {new ConstantParameter("movies1.txt")};
    pico.registerComponentImplementation(MovieFinder.class, ColonMovieFinder.class, finderParams); // implementation을 어떤 class로 사용할 것인지 ..
    pico.registerComponentImplementation(MovieLister.class); 
    return pico;
}

```

만약 다른 사람이 MovieListerㅇㅔ 다른 finder를 사용하려면? 


```java
private MutablePicoContainer configureContainer() {
    MutablePicoContainer pico = new DefaultPicoContainer();
    Parameter[] finderParams =  {new ConstantParameter("movies.xml")}; //xml을 사용한다.
    pico.registerComponentImplementation(MovieFinder.class, XmlMovieFinder.class, finderParams); // implementation을 어떤 class로 사용할 것인지 ..
    pico.registerComponentImplementation(MovieLister.class); 
    return pico;
}

```

### Setter Injection with Spring

- Spring 은 setter / constructor 제공함

```java
class MovieLister...

  private MovieFinder finder;
    public void setFinder(MovieFinder finder) {
      this.finder = finder;
    }

```
```java
class ColonMovieFinder...

  public void setFilename(String filename) {
      this.filename = filename;
  }

```
- Class에 setter가 존재해야 함

PicoContainer에서 code로 했던 것과 같이, 이번에는 xml configuration file로 의존성 관계를 명시함. (Spring도 코드로 가능, annotation으로도 가능)

```xml
<beans>
    <bean id="MovieLister" class="spring.MovieLister">
        <property name="finder">
            <ref local="MovieFinder"/>
        </property>
    </bean>
    <bean id="MovieFinder" class="spring.ColonMovieFinder">
        <property name="filename">
            <value>movies1.txt</value>
        </property>
    </bean>
</beans>
```



```java
public void testWithSpring() throws Exception {
    ApplicationContext ctx = new FileSystemXmlApplicationContext("spring.xml");//어떤 종류의 configuration을 사용한 applicationContext를 사용할 것인지 명시
    MovieLister lister = (MovieLister) ctx.getBean("MovieLister");
    Movie[] movies = lister.moviesDirectedBy("Sergio Leone");
    assertEquals("Once Upon a Time in the West", movies[0].getTitle());
}
```

### Interface Injection

- Avalon이라는 framework이 사용함
- Inject를 받도록 interface를 선언하여 사용

```java
public interface InjectFinder { //MovieFinder를 받는 inject interface를 사용
    void injectFinder(MovieFinder finder);
}

class MovieLister implements InjectFinder 

  public void injectFinder(MovieFinder finder) {
      this.finder = finder;
  }
```


```java
public interface InjectFinderFilename {
    void injectFilename (String filename);
}
class ColonMovieFinder implements MovieFinder, InjectFinderFilename...

  public void injectFilename(String filename) {
      this.filename = filename;
  }

```

- Register Components
- Register Injectors


```java
  private Container container;

   private void configureContainer() {
     container = new Container();
     registerComponents();
     registerInjectors();
     container.start();
  }
```

```java
class Tester...

//component 등록
  private void registerComponents() {
    container.registerComponent("MovieLister", MovieLister.class);
    container.registerComponent("MovieFinder", ColonMovieFinder.class);
  }
//register injectors
private void registerInjectors(){
    container.registerInjector(InjectFinder.class, container.lookup("MovieFinder"));
    container.registerInjector(InjectFinderFilename.class, new FinderFilenameInjector()));
    //String을 inject할 generic injector

}
```

최종적으로 사용시

```java
class Tester…

  public void testIface() {
    configureContainer();
    MovieLister lister = (MovieLister)container.lookup("MovieLister");
    Movie[] movies = lister.moviesDirectedBy("Sergio Leone");
    assertEquals("Once Upon a Time in the West", movies[0].getTitle());
  }
```

container는 MovieLister가InjectFinder를 구현하고, InjectFinder는 MovieFinder라는 component를 inject받는 것을 알 수 있음 ( registerInjectors에 configure )


## Using Service Locator

To have an Object that knows how to get hold of all of the services that an application might need

<img src="https://www.martinfowler.com/articles/injection/locator.gif">

DI에서는 movieLister에 대한 DI 설정을 해주고, movieFinder와 wiring 작업을 수행함
최종적으로 사용되는 service에대한 instanization 자체가 어떻게 이루어지는에 대한 차이
(https://stackoverflow.com/questions/1557781/whats-the-difference-between-the-dependency-injection-and-service-locator-patte)
ServiceLocator는 client가 여전히 dependency를 생성해야함, 그리고 servicelocator의 어떤 특정 method를 통해 호출해야 함 


```java
class MovieLister...

  MovieFinder finder = ServiceLocator.movieFinder();  // 여기가 결정적인 차이점, dependency의 결정을 movieLister가 하는 것이 아닌, serviceLocator에 위임한다

class ServiceLocator...

  public static MovieFinder movieFinder() {
      return soleInstance.movieFinder; //service Locator는 movieFinder를 준비했다가, 제공함
  }
  private static ServiceLocator soleInstance; //singleton

```
```java

class Tester...

  private void configure() {
      ServiceLocator.load(new ServiceLocator(new ColonMovieFinder("movies1.txt"))); // serviceLocator는 어떤 movieFinder를 제공해야 하는지 알고 있음
  }
class ServiceLocator...

  public static void load(ServiceLocator arg) {
      soleInstance = arg;
  }

  public ServiceLocator(MovieFinder movieFinder) {
      this.movieFinder = movieFinder;
  }

```

### A Dynamic Service Locator

- ServiceLocator의 movieFinder()는 static 한 method로 service를 사용함
- Dynamic하게 runtime에 선택하여 호출도 가능하게 할 수 있음
- 보면 applicationContext (spring)에서 getBeand을 dynamic하게 사용하는 방식임 

```java
class ServiceLocator...

  private static ServiceLocator soleInstance;
  public static void load(ServiceLocator arg) {
      soleInstance = arg;
  }
  private Map services = new HashMap();
  public static Object getService(String key){
      return soleInstance.services.get(key);
  }
  public void loadService (String key, Object service) {
      services.put(key, service);
  }

```
```java
class Tester...

  private void configure() {
      ServiceLocator locator = new ServiceLocator();
      locator.loadService("MovieFinder", new ColonMovieFinder("movies1.txt"));
      ServiceLocator.load(locator);
  }

MovieFinder finder = (MovieFinder) ServiceLocator.getService("MovieFinder");// (MovieFinder)ctx.getBean("Moviefinder")

```

### Service Locator in Spring 

#### Factory Bean

- Factory 역할을 하는 bean 
- FactoryBean도 bean인지라 BeanFactory(를 상속한 applicationContext)에 등록해서 사용
- 이름이 비슷한거지.. Factory 역할을 하는 bean이다 
- FactoryBean 경우에 self가 bean instance로 사용되는 것이 아니라, 만드는 objet의 factory 목적으로 사용됨
- singleton /prototype support 그리고, eager, lazy 가능 
- IOC Container는 FactoryBean의 life cycle에 관여하지만, factorybean이 만드는 object는 관여안함
- 따라서 알아서 disposableBean을 구현하거나 .. 알아서. .

> Interface to be implemented by objects used within a BeanFactory which are themselves factories for individual objects. If a bean implements this interface, it is used as a factory for an object to expose, not directly as a bean instance that will be exposed itself.
> FactoryBeans can support singletons and prototypes, and can either create objects lazily on demand or eagerly on startup. The SmartFactoryBean interface allows for exposing more fine-grained behavioral metadata.
> The container is only responsible for managing the lifecycle of the FactoryBean instance, not the lifecycle of the objects created by the FactoryBean. Therefore, a destroy method on an exposed bean object (such as Closeable.close()) will not be called automatically. Instead, a FactoryBean should implement DisposableBean and delegate any such close call to the underlying objec


https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/beans/factory/FactoryBean.html


#### ServiceLocatorFactoryBean

- FactoryBean의 한 종류
- 그러므로 당연히 Bean을 만들어서 주는 역할을 한다 
- prototype bean을 만드는데 주로 사용함
- 보통 client에서 bean을 호출하는데, bean이 없으면 NoSuchBeanDefinitionException 나옴
- MyType xxx() 나 MyType xxx(id)를 구현할 경우, 이거에 맞춰서 dynamic proxy 를 생성하고, 이걸 BeanFactory로 넘김

```java

package a.b.c;

public interface ServiceFactory {

   //must have one or more methods with the signature MyType xxx(), MytType xxx
    public MyService getService();
    //creates dynamic proxy
}
```
```xml

<beans>

   <!-- Prototype bean since we have state -->
   <bean id="myService" class="a.b.c.MyService" singleton="false"/>

   <!-- will lookup the above 'myService' bean by *TYPE* -->
   <bean id="myServiceFactory"
            class="org.springframework.beans.factory.config.ServiceLocatorFactoryBean">
     <property name="serviceLocatorInterface" value="a.b.c.ServiceFactory"/>
   </bean>

   <bean id="clientBean" class="a.b.c.MyClientBean">
     <property name="myServiceFactory" ref="myServiceFactory"/>
   </bean>

</beans>

```

```java

package a.b.c;

public class MyClientBean {

    private ServiceFactory myServiceFactory;

    // actual implementation provided by the Spring container
    public void setServiceFactory(ServiceFactory myServiceFactory) {
        this.myServiceFactory = myServiceFactory;
    }

    public void someBusinessMethod() {
        // get a 'fresh', brand new MyService instance
        MyService service = this.myServiceFactory.getService();
        // use the service object to effect the business logic...
    }
}
```

조건을 넣어서 동적으로 찾는 경우도 가능

```java
package a.b.c;

public interface ServiceFactory { 

   // MyType xxx() || MyType xxx(MyIdType id)

    public MyService getService (String serviceName);
}

```
```xml
<beans>

   <!-- Prototype beans since we have state (both extend MyService) -->
   <bean id="specialService" class="a.b.c.SpecialService" singleton="false"/>
   <bean id="anotherService" class="a.b.c.AnotherService" singleton="false"/>
   <!-- id가 specailService/anotherService--> 

   <bean id="myServiceFactory"
            class="org.springframework.beans.factory.config.ServiceLocatorFactoryBean">
     <property name="serviceLocatorInterface" value="a.b.c.ServiceFactory"/>
   </bean>

   <bean id="clientBean" class="a.b.c.MyClientBean">
     <property name="myServiceFactory" ref="myServiceFactory"/>
   </bean>

</beans>

```
```java
package a.b.c;

public class MyClientBean {

    private ServiceFactory myServiceFactory;

    // actual implementation provided by the Spring container
    public void setServiceFactory(ServiceFactory myServiceFactory) {
        this.myServiceFactory = myServiceFactory;
    }

    public void someBusinessMethod() {
        // get a 'fresh', brand new MyService instance
        MyService service = this.myServiceFactory.getService("specialService");
        // use the service object to effect the business logic...
    }

    public void anotherBusinessMethod() {
        // get a 'fresh', brand new MyService instance
        MyService service = this.myServiceFactory.getService("anotherService");
        // use the service object to effect the business logic...
    }
}

```

https://springframework.guru/service-locator-pattern-in-spring/


## ServiceLocator vs DependencyInjection

Using Service Locator, can hide dependencies to other implementations(doesn't care about which dependency is used to service, dependency가 변경, 추가 , 삭제 되더라도 client 코드는 touch가 없다)
But every client code must see ServiceLocator

Using dependency injection can help make it easier to see what the component dependencies are. 
With dependency injector you can just look at the injection mechanism, such as the constructor, and see the dependencies. With the service locator you have to search the source code for calls to the locator. Modern IDEs with a find references feature make this easier, but it's still not as easy as looking at the constructor or setting methods.

- 사용하는 쪽이 한정되어 있다면, ServiceLocator pattern도 그다지 문제는 없다.
- 다만, 사용하는 쪽의 api를 염두에 두고 개발할 것이 아니라면, DI가 좋다. 사용하는 쪽에서 ServiceLocator를 이미 가지고 있을 수 있고, 이게 내가만든 ServiceLocator가 호환이 될 수 없으면(만약 그쪽도 나름대로의 movieFinder()의 method가 존재했다면? adapter pattern으로 감싸도.. 여전히 내 serviceLocator에 의존적이게 된다) 
- 사람들이 test가 쉽다는 이유도 들고는 하는데, serviceLocator도 interface화 한다음에, test용 ServiceLocator를 Implementation하면 되서 딱히 상관없는 듯


## Constructor injection vs Setter injection 

- Create valid objects at construction time 
- Constructor를 사용한 initialization은 명백한 시기에 valid한 object를 만들 수 있도록 해준다.
- 그리고 다양한 parameter로 valid object를 정의할 수 있다(constructor overloading)
- immutable하게 선언할 수 있다. final 사용해서 
- 길어지면 보기싫긴 하다. 
- 그리고 string 같은 primitive type injet 받을 때 여러개라면, position으로 구분될 수 밖에 없다(method 내에 구분은 되어도 , DI가 이루어질 때.., 그래서 spring에서는 parameter name으로 넣는 방식이 있다)

### Spring Doc에서 constructor injection을 추천하는 이유

- Immutable 하게 해줌
- NotNull 보장가능
- Fully initialized State로 return 되기 때문에 안정적임
- Setter를 사용하면 나중에 set으로 다시 inject가능하다는 게 장점임 

> The Spring team generally advocates constructor injection, as it lets you implement application components as immutable objects and ensures that required dependencies are not null. Furthermore, constructor-injected components are always returned to the client (calling) code in a fully initialized state. As a side note, a large number of constructor arguments is a bad code smell, implying that the class likely has too many responsibilities and should be refactored to better address proper separation of concerns.

> Setter injection should primarily only be used for optional dependencies that can be assigned reasonable default values within the class. Otherwise, not-null checks must be performed everywhere the code uses the dependency. One benefit of setter injection is that setter methods make objects of that class amenable to reconfiguration or re-injection later. Management through JMX MBeans is therefore a compelling use case for setter injection.

> Use the DI style that makes the most sense for a particular class. Sometimes, when dealing with third-party classes for which you do not have the source, the choice is made for you. For example, if a third-party class does not expose any setter methods, then constructor injection may be the only available form of DI.




## Code or Configuration files

- 가끔 강성으로 configuration file을 써야한다는 쪽이 있는데 상황 맞춰서 쓰면됨 



ref : 

https://www.martinfowler.com/articles/injection.html




