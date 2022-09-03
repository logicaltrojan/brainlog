
### What do you understand by Dependency Injection?

![IMAGE](https://s3.ap-south-1.amazonaws.com/myinterviewtrainer-domestic/public_assets/assets/000/000/841/original/Dependency_Injection.png?1628693939))

The main idea in Dependency Injection is that you don’t have to create your objects but you just have to describe how they should be created.


### FactoryBean vs BeanFactory

BeanFactory -> IoC container interface , ApplicationContext extends BeanFactory
XMLBeanFactory... 

FactoryBean -> Interface that use to make Bean, Developer have to implement interface 


Factory Method -> Method that instaniates Bean - https://www.baeldung.com/spring-beans-factory-methods


```java
package com.spring.factory;
 
import org.springframework.beans.factory.FactoryBean;
 
import com.mesage.provider.HelloMessageProvider;
import com.mesage.provider.HiMessageProvider;
import com.mesage.provider.IMessageProvider;
 
public class MessageFactoryBean implements FactoryBean<IMessageProvider> {
 
    private String type;
    
    @Override
    public IMessageProvider getObject() throws Exception {
        
        if("Hi".equals(type))
            return HiMessageProvider.getInstance();
        
        else 
            return HelloMessageProvider.getInstance();
    }
 
    @Override
    public Class<IMessageProvider> getObjectType() {
        return IMessageProvider.class;
    }
 
    @Override
    public boolean isSingleton() {
        return true;
    }
 
    public void setType(String type) {
        this.type = type;
    }
}

```
출처: https://javaslave.tistory.com/53 [전산쟁이 블로그]

### Why Constructor injection better than field injection

1. Setter injection -> Dev can make a mistake while initailizing
```java
@Controller public class UserController { @GetMapping("/users/{id}") public User getUserInfo(@PathVariable long id) { UserService userService = new UserService(); // userService.setUserRepository(new UserRepository()); return userService.getUser(id); } }

```
출처: https://interconnection.tistory.com/124 [Ryan Server]


2. All Required Dependencies Are Available at Initialization Time
-> can figure out circular references? 

3. Immutability


### Explain Bean life cycle in Spring Bean Factory Container

![IMAGE](https://s3.ap-south-1.amazonaws.com/myinterviewtrainer-domestic/public_assets/assets/000/000/842/original/Spring_Bean_Life_Cycle.png?1628694921))

1. The IoC container instantiates the bean from the bean’s definition in the XML file.
2. Spring then populates all of the properties using the dependency injection as specified in the bean definition.
3. The bean factory container calls setBeanName() which take the bean ID and the corresponding bean has to implement BeanNameAware interface.
4. The factory then calls setBeanFactory() by passing an instance of itself (if BeanFactoryAware interface is implemented in the bean).
5. If BeanPostProcessors is associated with a bean, then the preProcessBeforeInitialization() methods are invoked.
6. If an init-method is specified, then it will be called.
7. Lastly, postProcessAfterInitialization() methods will be called if there are any BeanPostProcessors associated with the bean that needs to be run post creation.