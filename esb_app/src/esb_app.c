#include <kore/kore.h>
#include <kore/http.h>

int		page(struct http_request *);

int
page(struct http_request *req)
{	printf("\n");
	printf("=================welcome to esb_app===============\n");
	http_response(req, 200, NULL, 0);
	return (KORE_RESULT_OK);
}
