# DispatcherServlet

Root WebApplicationContext : ContextLoaderListener 에 의해 등록된 IOC container
Servlet WebApplicationContext : DispatcherServlet이 상속받아 만드는 container, only usable in DispatcherServlet

```java
    @Nullable
    private MultipartResolver multipartResolver;
    @Nullable
    private LocaleResolver localeResolver;
    @Nullable
    private ThemeResolver themeResolver;
    @Nullable
    private List<HandlerMapping> handlerMappings;
    @Nullable
    private List<HandlerAdapter> handlerAdapters;
    @Nullable
    private List<HandlerExceptionResolver> handlerExceptionResolvers;
    @Nullable
    private RequestToViewNameTranslator viewNameTranslator;
    @Nullable
    private FlashMapManager flashMapManager;
    @Nullable
    private List<ViewResolver> viewResolvers;
```

Bean Type Explanation
HandlerMapping
요청을 핸들러에 매핑한다. 핸들러는 전처리나 후처리를 위한 Interceptor를 가질 수 있다. HandlerMapping의 구현 디테일에 따라 매핑된다. HandlerMapping의 두 주요 구현체는 RequestMappingHandlerMapping (@RequestMapping), SimpleUrlHandlerMapping이다.

HandlerAdapter	DispatcherServlet이 매핑된 핸들러를 호출하는 것을 돕는다. 핸들러가 실제로 호출되는것과 관계 없이 수행된다.(Adapter에 주목)HandlerAdapter의 주된 목표는 DispatcherServlet이 요청을 위임하는데 추상화를 제공하는 것이다.

HandlerExceptionResolver	
요청을 핸들러에 매핑하거나, HTML에러를 보여주거나, 기타 동작 중에 예외를 인지하고 결정하는 전략을 담당한다.

ViewResolver	
핸들러가 반환한 문자열 기반으로 뷰 네임을 결정하고 응답을 렌더링한다.

LocaleResolver	
글로벌 서비스를 제공할 수 있도록, Locale을 결정한다.

ThemeResolver	
어플리케이션이 사용할 수 있는 테마를 결정한다.

MultipartResolver	
Multi-part 요청(파일 요청)에 대해 추상화된 파싱방법 또는 파싱 라이브러리를 제공한다.

FlashMapManager	
input, output FlashMap을 저장하고 불러온다. FlashMap은 어떤 요청으로부터 다른 요청으로 attributes들을 전달할 수 있는데, 주로 Redirect에 사용된다.



Handler Mapping 

BeanNameUrlHandlerMapping
"/" in bean name

```java
@Bean("/accounts")
public AccountController accountController() {
    return new AccountController();
}
```

SimpleUrlHandlerMapping

```java
@Bean
SimpleUrlHandlerMapping urlHandlerMapping() {
    SimpleUrlHandlerMapping simpleUrlHandlerMapping = new SimpleUrlHandlerMapping();
    simpleUrlHandlerMapping.setOrder(Ordered.LOWEST_PRECEDENCE - 2);

    Map<String, Object> mapping = new HashMap<>();
    mapping.put("/accounts", accountController());
    simpleUrlHandlerMapping.setUrlMap(mapping);
//or 
//  Properties properties = new Properties();
//  properties.put("/accounts", "accountController");
//  simpleUrlHandlerMapping.setMappings(properties);

    return simpleUrlHandlerMapping;
}
```

RequestMappingHandlerMapping
```java
@Controller
@RequestMapping("/accounts")
public class AccountController {

}
```


Handler Adapter
After HandlerMapping, invokes Handler Method 

SimpleControllerHandlerAdapter
```java
public class AccountController implements Controller {

    @Override
    public ModelAndView handleRequest(HttpServletRequest request, HttpServletResponse response) throws Exception {
        ///... 
        return null;
    }
}
```
HttpRequestHandlerAdapter
```java
public class AccountController implements HttpRequestHandler {

    @Override
    public void handleRequest(HttpServletRequest request, HttpServletResponse response)  {
        ///...
    }
}
```
SimpleServletHandlerAdapter
```java
public class AccountController extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

    }
}
```
RequestMappingHandlerAdapter

Unlinkely as Adapters above, the RequestMappingHandlerAdapter doesn't require to implement certain interface. So the adapter has use Reflection. 
```java
@Controller
@RequestMapping("/accounts")
public class AccountController {


    @GetMapping("/{id}")
    public String hello(@PathVariable Long id) {

       //...
    }
}


```


DispatcherServlet 은 다음과 같이 요청을 처리한다.

```java
protected void doService(HttpServletRequest request, HttpServletResponse response) throws Exception {
        this.logRequest(request);
        Map<String, Object> attributesSnapshot = null;
        //...
        
        request.setAttribute(WEB_APPLICATION_CONTEXT_ATTRIBUTE, this.getWebApplicationContext()); // 1
        request.setAttribute(LOCALE_RESOLVER_ATTRIBUTE, this.localeResolver); //2
        request.setAttribute(THEME_RESOLVER_ATTRIBUTE, this.themeResolver);
        request.setAttribute(THEME_SOURCE_ATTRIBUTE, this.getThemeSource());
        //3
      
        //...
        try {
            this.doDispatch(request, response);
        } finally {

        }

    }

```
```java
  protected void doDispatch(HttpServletRequest request, HttpServletResponse response) throws Exception {
        HttpServletRequest processedRequest = request;
        HandlerExecutionChain mappedHandler = null;
        boolean multipartRequestParsed = false;
        WebAsyncManager asyncManager = WebAsyncUtils.getAsyncManager(request);

        try {
            try {
                ModelAndView mv = null;
                Object dispatchException = null;

                try {
                    processedRequest = this.checkMultipart(request); // 4
                    multipartRequestParsed = processedRequest != request;
                    mappedHandler = this.getHandler(processedRequest); //5
                    if (mappedHandler == null) {
                        this.noHandlerFound(processedRequest, response);
                        return;
                    }

                    HandlerAdapter ha = this.getHandlerAdapter(mappedHandler.getHandler()); //5
                    String method = request.getMethod();
                    boolean isGet = HttpMethod.GET.matches(method);
                    if (isGet || HttpMethod.HEAD.matches(method)) {
                        long lastModified = ha.getLastModified(request, mappedHandler.getHandler());
                        if ((new ServletWebRequest(request, response)).checkNotModified(lastModified) && isGet) {
                            return;
                        }
                    }

                    if (!mappedHandler.applyPreHandle(processedRequest, response)) {
                        return;
                    }

                    mv = ha.handle(processedRequest, response, mappedHandler.getHandler());
                    if (asyncManager.isConcurrentHandlingStarted()) {
                        return;
                    }

                    this.applyDefaultViewName(processedRequest, mv); //6
                    mappedHandler.applyPostHandle(processedRequest, response, mv);
                } catch (Exception var20) {
         
                }

                this.processDispatchResult(processedRequest, response, mappedHandler, mv, (Exception)dispatchException);
            } catch (Exception var22) {
          
            }

        } finally {
            if (asyncManager.isConcurrentHandlingStarted()) {
                if (mappedHandler != null) {
                    mappedHandler.applyAfterConcurrentHandlingStarted(processedRequest, response);
                }
            } else if (multipartRequestParsed) {
                this.cleanupMultipart(processedRequest);
            }

        }
    }


```


1. 컨트롤러나 기타 요소가 사용할 WebApplicationContext를 요청에 바인딩한다. 기본으로 DispatcherServlet.WEB_APPLICATION_CONTEXT_ATTRIBUTE 키에 바인딩된다.
2. LocaleResolver를 요청에 바인딩한다. Locale은 요청을 처리할 때 (View 렌더링 등)에 사용된다. Locale 을 결정할 필요 없다면 LocaleResolver가 없어도 된다.
3. ThemeResolver를 요청에 바인딩한다. ThemeResolver는 뷰와 같은 요소가 테마를 결정할 때 사용한다. 테마를 사용하지 않으면 무시해도 된다.
4. 특정한 MultipartFileResolver를 지정했다면 요청에서 파일을 검사한다. 파일이 발견되면 HttpServletRequest를 추가 처리를 위해 MultipartHttpServletRequest로 래핑한다.
5. 적절한 핸들러를 찾는다. 핸들러를 찾으면 모델 또는 렌더링을 준비하기 위해 핸들러와 연관된 실행 흐름(execution chain), preprocessor, postprocessor, controller 등을 실행한다. 또 애너테이션이 부여된 컨트롤러의 경우 뷰를 반환하는 대신 HandlerAdapter 내에서 응답을 처리할 수도 있다.
6. 핸들러가 모델을 리턴하면 뷰가 렌더링된다. 모델이 리턴되지 않으면 (요청을 intercept하는 preprocessor 나 postprocessor 때문일 수 있음) 요청이 이미 이행되었을 수 있으므로, 뷰가 렌더링되지 않는다.
7. WebApplicationContext가 관리하는 HandlerExceptionResolver 빈은 위의 요청을 처리하는 중에 발생한 예외를 처리한다.