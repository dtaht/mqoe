//#include "web.h"

#include <sys/types.h> /* size_t, ssize_t */
#include <stdarg.h> /* va_list */
#include <stddef.h> /* NULL */
#include <stdint.h> /* int64_t */
#include <kcgi.h>

struct callbacks {
		char *route;
		(*cbk);
};

struct callbacks * a = {
		{ "/api/index", index() },
		{ "/api/json", json() },
		{ 0, done() }
}

int main(void) {
    struct kreq r;
    const char *page = "index";
    if (khttp_parse(&r, NULL, 0, &page, 1, 0) != KCGI_OK)
        return 1;
    khttp_head(&r, kresps[KRESP_STATUS],
        "%s", khttps[KHTTP_200]);
    khttp_head(&r, kresps[KRESP_CONTENT_TYPE],
        "%s", kmimetypes[KMIME_TEXT_PLAIN]);
    khttp_body(&r);
    khttp_puts(&r, "Hello, world!");
    khttp_free(&r);
    return 0;
}
