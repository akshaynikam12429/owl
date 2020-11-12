#include <stdio.h>
#include "../test/munit.h"
#include "stdlib.h"
#include "esb.h"
#include<dirent.h>
#include<string.h>
#include "../adapter/email.h" 
#include "ftp.h"
#include "http.h"
#include"../adapter/transform.h"
#include "../adapter/transform_to_csv.h"

//gcc test_esb.c ../test/munit.c ftpp.c  esb.c http.c ../adapter/email.c ../adapter/xmltojson.c   `mysql_config --cflags --libs` `xml2-config --cflags --libs` -lcurl -ljson-c  -o test_esb
/* This is just to disable an MSVC warning about conditional
 * expressions being constant, which you shouldn't have to do for your
 * code.  It's only here because we want to be able to do silly things
 * like assert that 0 != 1 for our demo. */
#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

static void * test_xml_values_setup(const MunitParameter params[], void *user_data)
{
    /**
     * Return the data that will be used for test_xml_values. Here we
     * are just return a string. It can be a struct or anything.
     * The memory that you allocate here for the test data
     * has to be cleaned up in corresponding tear down function,
     * which in this case is test_tear_down.
     */
    return strdup("/home/akshay/owl/esb_proj/esb_app/src/esb/bmd.xml");
}

static void test_xml_values_tear_down(void *fixture)
{
    /* Receives the pointer to the data if that that was created in
    test_setup function. */
    free(fixture);
}

// For testing the bmd values
static MunitResult test_xml_values(const MunitParameter params[], void* fixture) {
  char *path = (char *)fixture;

 


  BMD *test_bmd= parse_bmd_xml(path);

  printf("%s\n" ,test_bmd->bmd_envelope->MessageID);

  if(strcmp(path,"/home/akshay/owl/esb_proj/esb_app/src/esb/bmd.xml")==0) { 
    //compare each and every data of xml with the expected data
    munit_assert_string_equal(test_bmd->bmd_envelope->MessageID,"A9ECAEF2-107A-4452-9553-043B6D25386E");
    munit_assert_string_equal(test_bmd->bmd_envelope->MessageType,"CreditReport");
    munit_assert_string_equal(test_bmd->bmd_envelope->Sender,"756E2EAA-1D5B-4BC0-ACC4-4CEB66940");
    munit_assert_string_equal(test_bmd->bmd_envelope->Destination,"6393F82F-4687-433D-AA23-196633081FE");
    munit_assert_string_equal(test_bmd->bmd_envelope->CreationDateTime,"2020-08-12T05:18:00+0000");
    munit_assert_string_equal(test_bmd->bmd_envelope->ReferenceID,"INV-PROFILE-889712");
    munit_assert_string_equal(test_bmd->bmd_envelope->Signature,"63f5f61f7a79301f715433f8f3689390d1f5da4f855169023300491c00b8113c");
    munit_assert_string_equal(test_bmd->bmd_payload->data,"Hello");
    }

  
  

  return MUNIT_OK;
}


//for xml to json function
static MunitResult test_transform(const MunitParameter params[], void* fixture) {
  char * filp =transformjson("../../bmd_files/1605153008/bmd1.xml");
  printf("\n\nfilp is : %s\n\n\n",filp);
  munit_assert_string_equal(filp,"{\"Payload\":\"Hello\"}");
  return MUNIT_OK;
}

//for xml to csv function
static MunitResult test_CSVtransform(const MunitParameter params[], void* fixture) {
  BMD *test_bmd= parse_bmd_xml("/home/akshay/owl/esb_proj/esb_app/src/esb/bmd.xml");
  char * filp =transformCSV("test_bmd->bmd_payload->data");
  printf("\n\nfilp is : %s\n\n\n",filp);
  munit_assert_string_equal(filp,"payload_data.csv");
  return MUNIT_OK;
}





//For testing the validation of the BMD
static MunitResult test_bmd_valid(const MunitParameter params[], void* fixture) {
  //char *path = (char *)fixture;

  BMD *test_bmd= parse_bmd_xml("/home/akshay/owl/esb_proj/esb_app/src/esb/bmd.xml");

  printf("%s\n" ,test_bmd->bmd_envelope->MessageID);

  
  //validation test
  TD temp =is_bmd_valid(test_bmd);
  int val= temp.val;
  munit_assert_int(val,==,1);

  

  return MUNIT_OK;
}


//For testing the Queue request
static MunitResult test_queue_request(const MunitParameter params[], void* fixture) {
  

  BMD *test_bmd= parse_bmd_xml("/home/akshay/owl/esb_proj/esb_app/src/esb/bmd.xml");

  printf("%s\n" ,test_bmd->bmd_envelope->MessageID);

  
  //validation test
  munit_assert_int(queue_the_request(test_bmd),==,1);

  

  return MUNIT_OK;
}


//For testing the email service
static MunitResult
test_email_service(const MunitParameter params[], void * fixture) {
    int status = emailsender("akshaynikam12429@gmail.com", "bmd.xml");
    munit_assert_int(status, == , 1);
    return MUNIT_OK;
}


//For testing the FTP service
static MunitResult
test_ftp(const MunitParameter params[], void * fixture) {
    int status = send_ftp_file("ftp://akshaynikam@192.168.0.100/payload.json");
    munit_assert_int(status, == , 1);
    return MUNIT_OK;
}

//For testing the HTTP service
static MunitResult
test_http(const MunitParameter params[], void * fixture) {
    int status = http_request("http://10.0.2.15", "Hello");
    munit_assert_int(status, == , 1);
    return MUNIT_OK;
}




/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: Put all unit tests here. */
static MunitTest esb_tests[] = {
  
  
  { (char*) "/test_xml_values", test_xml_values, test_xml_values_setup , test_xml_values_tear_down, MUNIT_TEST_OPTION_NONE, NULL},
   
   { (char*) "/test_bmd_valid", test_bmd_valid, NULL , NULL, MUNIT_TEST_OPTION_NONE, NULL},

  { (char*) "/test_queue_request",test_queue_request, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

  { (char*) "/test_email_service",test_email_service, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },


 { (char*) "/test_ftp",test_ftp, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

 { (char*) "/test_http",test_http, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
 
 { (char*) "/test_transform",test_transform, NULL,NULL,MUNIT_TEST_OPTION_NONE, NULL },
 
 { (char*) "/test_CSVtransform",test_CSVtransform, NULL,NULL,MUNIT_TEST_OPTION_NONE, NULL },
 
 


  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};



/* Now we'll actually declare the test suite.  You could do this in
 * the main function, or on the heap, or whatever you want. */
static const MunitSuite test_suite = {
  /* This string will be prepended to all test names in this suite;
   * for example, "/example/rand" will become "/µnit/example/rand".
   * Note that, while it doesn't really matter for the top-level
   * suite, NULL signal the end of an array of tests; you should use
   * an empty string ("") instead. */
  (char*) "",
  /* The first parameter is the array of test suites. */
  esb_tests,
  /* In addition to containing test cases, suites can contain other
   * test suites.  This isn't necessary in this example, but it can be
   * a great help to projects with lots of tests by making it easier
   * to spread the tests across many files.  This is where you would
   * put "other_suites" (which is commented out above). */
  NULL,
  /* An interesting feature of µnit is that it supports automatically
   * running multiple iterations of the tests.  This is usually only
   * interesting if you make use of the PRNG to randomize your tests
   * cases a bit, or if you are doing performance testing and want to
   * average multiple runs.  0 is an alias for 1. */
  1,
  /* Just like MUNIT_TEST_OPTION_NONE, you can provide
   * MUNIT_SUITE_OPTION_NONE or 0 to use the default settings. */
  MUNIT_SUITE_OPTION_NONE
};

/* This is only necessary for EXIT_SUCCESS and EXIT_FAILURE, which you
 * *should* be using but probably aren't (no, zero and non-zero don't
 * always mean success and failure).  I guess my point is that nothing
 * about µnit requires it. */
#include <stdlib.h>
#if 1
int main(int argc, char* argv[]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, NULL, argc, NULL );
}
#endif
