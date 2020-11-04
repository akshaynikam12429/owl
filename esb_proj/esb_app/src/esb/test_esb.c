#include <stdio.h>
#include "../test/munit.h"
#include "stdlib.h"
#include "esb.h"

//gcc test_esb.c munit.c  esb.c  `mysql_config --cflags --libs` `xml2-config --cflags --libs` -o test_esb//for compilation----------

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
    return strdup("/home/mukesh/bmd.xml");
}

static void test_xml_values_tear_down(void *fixture)
{
    /* Receives the pointer to the data if that that was created in
    test_setup function. */
    free(fixture);
}


static MunitResult test_xml_values(const MunitParameter params[], void* fixture) {
  char *path = (char *)fixture;

  

 /*
   * Possible values are:
   *  - MUNIT_OK: Sucess
   *  - MUNIT_FAIL: Failure
   *  - MUNIT_SKIP: The test was skipped; usually this happens when a
   *    particular feature isn't in use.  For example, if you're
   *    writing a test which uses a Wayland-only feature, but your
   *    application is running on X11.
   *  - MUNIT_ERROR: The test failed, but not because of anything you
   *    wanted to test.  For example, maybe your test downloads a
   *    remote resource and tries to parse it, but the network was
   *    down.
   */
  
  


  BMD *test_bmd= parse_bmd_xml(path);

  printf("%s\n" ,test_bmd->bmd_envelope->MessageID);

  if(strcmp(path,"/home/mukesh/bmd.xml")==0) { 
    munit_assert_string_equal(test_bmd->bmd_envelope->MessageID,"A049AEF2-107A-4452-9553-043B6D25386E");
    munit_assert_string_equal(test_bmd->bmd_envelope->MessageType,"DebitReport");
    munit_assert_string_equal(test_bmd->bmd_envelope->Sender,"556E2EAA-1D5B-5BC0-BCC4-4CEB669408DA");
    munit_assert_string_equal(test_bmd->bmd_envelope->Destination,"6323D82F-4687-433D-AA23-1966330381FE");
    munit_assert_string_equal(test_bmd->bmd_envelope->CreationDateTime,"2020-08-12T05:18:00+0000");
    munit_assert_string_equal(test_bmd->bmd_envelope->ReferenceID,"INV-PROFILE-889712");
    munit_assert_string_equal(test_bmd->bmd_envelope->Signature,"63f5f61f7a79301f715433f8f3689390d1f5da4f855169023300491c00b8113c");
    munit_assert_string_equal(test_bmd->bmd_payload->data,"HDFC0007498");
    }

  
  

  return MUNIT_OK;
}


static void * test_bmd_valid_setup(const MunitParameter params[], void *user_data)
{
    
    return strdup("/home/mukesh/bmd.xml");
}

static void test_bmd_valid_tear_down(void *fixture)
{
    free(fixture);
}


static MunitResult test_bmd_valid(const MunitParameter params[], void* fixture) {
  char *path = (char *)fixture;

  BMD *test_bmd= parse_bmd_xml(path);

  printf("%s\n" ,test_bmd->bmd_envelope->MessageID);

  
  //validation test
  munit_assert_int(is_bmd_valid(test_bmd),==,1);

  

  return MUNIT_OK;
}


/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest esb_tests[] = {
  
  { (char*) "/test_xml_values", test_xml_values, test_xml_values_setup , test_xml_values_tear_down, MUNIT_TEST_OPTION_NONE, NULL},
   
  { (char*) "/test_bmd_valid", test_bmd_valid, test_bmd_valid_setup , test_bmd_valid_tear_down, MUNIT_TEST_OPTION_NONE, NULL},

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
