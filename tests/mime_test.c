#include "mime.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "CuTest.h"
void TestGetFileMimeType(CuTest* tc) {
  const char* type = get_file_mime_type(".html");
  CuAssertStrEquals(tc, "text/html", type);

  type = get_file_mime_type(".xml");
  CuAssertStrEquals(tc, "text/xml", type);

  type = get_file_mime_type(".xhtml");
  CuAssertStrEquals(tc, "application/xhtml+xml", type);

  type = get_file_mime_type(".txt");
  CuAssertStrEquals(tc, "text/plain", type);

  type = get_file_mime_type(".rtf");
  CuAssertStrEquals(tc, "application/rtf", type);

  type = get_file_mime_type(".pdf");
  CuAssertStrEquals(tc, "application/pdf", type);

  type = get_file_mime_type(".png");
  CuAssertStrEquals(tc, "image/png", type);

  type = get_file_mime_type(".gif");
  CuAssertStrEquals(tc, "image/gif", type);

  type = get_file_mime_type(".jpg");
  CuAssertStrEquals(tc, "image/jpeg", type);

  type = get_file_mime_type(".jpeg");
  CuAssertStrEquals(tc, "image/jpeg", type);

  type = get_file_mime_type(".mpeg");
  CuAssertStrEquals(tc, "video/mpeg", type);

  type = get_file_mime_type(".avi");
  CuAssertStrEquals(tc, "video/x-msvideo", type);

  type = get_file_mime_type(".tar");
  CuAssertStrEquals(tc, "application/x-tar", type);

  type = get_file_mime_type(".css");
  CuAssertStrEquals(tc, "text/css", type);

  type = get_file_mime_type(".js");
  CuAssertStrEquals(tc, "text/javascript", type);

  type = get_file_mime_type(".csv");
  CuAssertStrEquals(tc, "text/csv", type);

  type = get_file_mime_type(".doc");
  CuAssertStrEquals(tc, "application/msword", type);

  type = get_file_mime_type(".json");
  CuAssertStrEquals(tc, "application/json", type);

  type = get_file_mime_type(".svg");
  CuAssertStrEquals(tc, "image/svg+xml", type);
}

CuSuite* mimeGetSuite() {
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, TestGetFileMimeType);
  return suite;
}