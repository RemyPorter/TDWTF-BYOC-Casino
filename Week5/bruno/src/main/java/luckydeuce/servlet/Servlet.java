package luckydeuce.servlet;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class Servlet extends HttpServlet {

    private ControllerProvider provider = new ControllerProvider();

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        work(req, resp);
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        work(req, resp);
    }

    private void work(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        String page = req.getRequestURI().substring(req.getRequestURI().lastIndexOf("/"));
        if (page.contains("?")) {
            page = page.substring(0, page.indexOf("?"));
        }
        BaseController c = provider.find(page);

        Map<String, String> values = new HashMap<String, String>();
        for (String p : c.getParameters()) {
            values.put(p, req.getParameter(p));
        }
        try {
            String response = c.handle(req, values);
            Map<String, Object> respObj = c.getResponse();
            for (String s : respObj.keySet()) {
                req.setAttribute(s, respObj.get(s));
            }
            req.getRequestDispatcher("/" + response).include(req, resp);
        } catch (Throwable e) {
            req.setAttribute("error", e.toString());
            try {
                req.getRequestDispatcher("/error.jsp").include(req, resp);
            } catch (ServletException ex) {
                
            }
        }
    }
}
