

<img src=https://stargatex.files.wordpress.com/2015/12/springmvcrequestlifecycle.jpg>
<img src=https://blog.kakaocdn.net/dn/MRXX3/btqK7014LOZ/wMKgG9RVkdZR7Ag5nD4wh0/img.png>




To use Spring MVC

Add dependency 
```xml
  <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-webmvc</artifactId>
      <version>5.3.8</version>
    </dependency>
  </dependencies>
```


web.xml
```xml
  <context-param>
    <param-name>contextClass</param-name>
    <param-value>org.springframework.web.context.support.AnnotationConfigWebApplicationContext</param-value>
  </context-param>

  <context-param>
    <param-name>contextConfigLocation</param-name>
    <param-value>troy.AppConfig</param-value>
  </context-param>
  <listener>
    <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
  </listener>
```

ContextLoaderListener 

ServeletContext의 라이프 사이클에 맞추어 Add ApplicationContext to ServletContext

ContextLoader(ContextLoaderListener extends ContextLoader)

Set Application Context to Servlet Context

Creates ApplicationContext and sets to ServletContext

```java
  public WebApplicationContext initWebApplicationContext(ServletContext servletContext) {
        if (servletContext.getAttribute(WebApplicationContext.ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE) != null) {
            throw new IllegalStateException("Cannot initialize context because there is already a root application context present - check whether you have multiple ContextLoader* definitions in your web.xml!");
        } else {
            servletContext.log("Initializing Spring root WebApplicationContext");
            Log logger = LogFactory.getLog(ContextLoader.class);
            if (logger.isInfoEnabled()) {
                logger.info("Root WebApplicationContext: initialization started");
            }

```


ContextClass 

ContextLoader determines whicih class use to make applicationContext
```java
 protected Class<?> determineContextClass(ServletContext servletContext) {
        String contextClassName = servletContext.getInitParameter("contextClass");
        if (contextClassName != null) {
            try {
                return ClassUtils.forName(contextClassName, ClassUtils.getDefaultClassLoader());
            } catch (ClassNotFoundException var4) {
                throw new ApplicationContextException("Failed to load custom context class [" + contextClassName + "]", var4);
            }
        } else {
            contextClassName = defaultStrategies.getProperty(WebApplicationContext.class.getName());

            try {
                return ClassUtils.forName(contextClassName, ContextLoader.class.getClassLoader());
            } catch (ClassNotFoundException var5) {
                throw new ApplicationContextException("Failed to load default context class [" + contextClassName + "]", var5);
            }
        }
    }
```


Using application context in servlet by servletContext
```java
ApplicationContext attribute =(ApplicationContext) getServletContext().getAttribute(WebApplicationContext.ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE); 
attribute.getBean("HelloService", HelloSerivce.class);
```


