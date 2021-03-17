/*
 * @Author: your name
 * @Date: 2021-03-04 18:59:52
 * @LastEditTime: 2021-03-11 15:23:52
 * @LastEditors: Please set LastEditors
 * @Description: httprequest 单元测试， 测试发送buffer string，通过context.parseRequest进行分析
 * @FilePath: /muduo/muduo/net/http/tests/HttpRequest_unittest.cc
 */
#include "muduo/net/http/HttpContext.h"
#include "muduo/net/Buffer.h"

//#define BOOST_TEST_MODULE BufferTest
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using muduo::string;
using muduo::Timestamp;
using muduo::net::Buffer;
using muduo::net::HttpContext; // 位于namespace muduo::net 中的class HttpContext
using muduo::net::HttpRequest;


BOOST_AUTO_TEST_CASE(testParseRequestAllInOne)  // 测试样例 case
{
  HttpContext context;
  Buffer input; // 缓冲区
  input.append("GET /index.html HTTP/1.1\r\n"
       "Host: www.chenshuo.com\r\n"
       "\r\n");

  BOOST_CHECK(context.parseRequest(&input, Timestamp::now()));  // 发送以上请求
  BOOST_CHECK(context.gotAll());
  const HttpRequest& request = context.request();
  BOOST_CHECK_EQUAL(request.method(), HttpRequest::kGet);   // 断言表达式必须成功
  BOOST_CHECK_EQUAL(request.path(), string("/index.html"));
  BOOST_CHECK_EQUAL(request.getVersion(), HttpRequest::kHttp11);
  BOOST_CHECK_EQUAL(request.getHeader("Host"), string("www.chenshuo.com"));
  BOOST_CHECK_EQUAL(request.getHeader("User-Agent"), string(""));
}

BOOST_AUTO_TEST_CASE(testParseRequestInTwoPieces)
{
  string all("GET /index.html HTTP/1.1\r\n"
       "Host: www.chenshuo.com\r\n"
       "\r\n");

  for (size_t sz1 = 0; sz1 < all.size(); ++sz1)
  {
    HttpContext context;
    Buffer input;
    input.append(all.c_str(), sz1);
    BOOST_CHECK(context.parseRequest(&input, Timestamp::now()));
    BOOST_CHECK(!context.gotAll());

    size_t sz2 = all.size() - sz1;
    input.append(all.c_str() + sz1, sz2);
    BOOST_CHECK(context.parseRequest(&input, Timestamp::now()));
    BOOST_CHECK(context.gotAll());
    const HttpRequest& request = context.request();
    BOOST_CHECK_EQUAL(request.method(), HttpRequest::kGet);
    BOOST_CHECK_EQUAL(request.path(), string("/index.html"));
    BOOST_CHECK_EQUAL(request.getVersion(), HttpRequest::kHttp11);
    BOOST_CHECK_EQUAL(request.getHeader("Host"), string("www.chenshuo.com"));
    BOOST_CHECK_EQUAL(request.getHeader("User-Agent"), string(""));
  }
}

BOOST_AUTO_TEST_CASE(testParseRequestEmptyHeaderValue)
{
  HttpContext context;
  Buffer input;
  input.append("GET /index.html HTTP/1.1\r\n"
       "Host: www.chenshuo.com\r\n"
       "User-Agent:\r\n"
       "Accept-Encoding: \r\n"
       "\r\n");

  BOOST_CHECK(context.parseRequest(&input, Timestamp::now()));
  BOOST_CHECK(context.gotAll());
  const HttpRequest& request = context.request();
  BOOST_CHECK_EQUAL(request.method(), HttpRequest::kGet);
  BOOST_CHECK_EQUAL(request.path(), string("/index.html"));
  BOOST_CHECK_EQUAL(request.getVersion(), HttpRequest::kHttp11);
  BOOST_CHECK_EQUAL(request.getHeader("Host"), string("www.chenshuo.com"));
  BOOST_CHECK_EQUAL(request.getHeader("User-Agent"), string(""));
  BOOST_CHECK_EQUAL(request.getHeader("Accept-Encoding"), string(""));
}
