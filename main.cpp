#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <curl/curl.h>
#define filename "code.jpg"

int download_pic(const char* url);
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
int main(int argc, char* argv[])
{
	char *outText;
#if DEBUG
	if(argc<2){
		printf("api <图片网址>\n");
		return 0;
	}
#else
	printf("content-type:text/html;charset=utf8\n\n");
	char *method;
	method = getenv("REQUEST_METHOD");
	if (method==NULL) {
		printf("REQUEST_METHOD,请求模式为空\n");
		return -1;
	}
	if (strcmp(method, "POST")!=0){
		printf("仅支持post");
		return -1;
	}
	int len = atoi(getenv("CONTENT_LENGTH"));
	char* data = (char*) malloc(sizeof(char)*(len+1));
	scanf("%s", data);
#endif


#if DEBUG
#else
	//if(download_pic(argv[1])<0) //网址模式
	if(download_pic(data)<0)

	{	printf("下载失败\n");
		return -1;
	}
#endif
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	// 指定使用英文
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	// api->Init([dataPath cStringUsingEncoding:NSUTF8StringEncoding], "eng");
	// Open input image with leptonica library
#if DEBUG
	Pix *image = pixRead(argv[1]);
#else
	Pix *image = pixRead(filename);
#endif
	api->SetImage(image);
	// 设置可识别出来的字符
	api->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZ:");
	//api->Recognize(0);
	//设置识别区域 x,y,宽,高
	api->SetRectangle(0, 0, 700, 15);
	//api->SetPageSegMode(tesseract::PSM_AUTO_OSD);
	// Get OCR result
	//得到字符
	outText = api->GetUTF8Text();
	printf("%s", outText);

	// Destroy used object and release memory
	api->End();
	delete[] outText;
	pixDestroy(&image);

	return 0;

}
/**
 * 使用curl库下载图片到服务器,供识别使用.
 * @param url
 * @return
 */
int download_pic(const char* url)
{
	CURL *curl;
	CURLcode res;
	//char *url;
	FILE *fp;
	curl = curl_easy_init();
	if (curl) {
		fp = fopen(filename, "w");
		if (fp==NULL) {
			curl_easy_cleanup(curl);
			return -1;
		}
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res!=CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		fclose(fp);
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return 0;
}
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)

{
	int written = fwrite(ptr, size, nmemb, (FILE *) stream);
	return written;
}
