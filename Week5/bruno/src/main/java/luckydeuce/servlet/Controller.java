package luckydeuce.servlet;

import java.util.List;
import java.util.Map;

public interface Controller {
    String handle(Map<String, String> params);
    List<String> getParameters();
    Map<String, Object> getResponse();
    boolean loginRequired();
}
