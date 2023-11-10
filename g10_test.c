/** !
 * Tester for g10
 * 
 * @file g10_test.c
 * 
 * @author Jacob Smith
 */

// Header
#include <sync/sync.h>
#include <log/log.h>
#undef NDEBUG
#include <g10/g10.h>
#include <g10/linear.h>
#include <g10/user_code.h>

// Enumeration definitions
enum result_e {
    zero      = 0,
    one       = 1,
    match     = 2,
    not_null  = 3,
    not_equal = 4
};

// Type definitions
typedef enum result_e result_t;

// Global variables
int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

// Forward declarations
/** !
 * Print the time formatted in days, hours, minutes, seconds, miliseconds, microseconds
 * 
 * @param seconds the time in seconds
 * 
 * @return void
 */
void print_time_pretty ( double seconds );

/** !
 * Run all the tests
 * 
 * @param void
 * 
 * @return void
 */
void run_tests ( void );

/** !
 * Print a summary of the test scenario
 * 
 * @param void
 * 
 * @return void
 */
void print_final_summary ( void );

/** !
 * Print the result of a single test
 * 
 * @param scenario_name the name of the scenario
 * @param test_name     the name of the test
 * @param passed        true if test passes, false if test fails
 * 
 * @return void
 */
void print_test ( const char *scenario_name, const char *test_name, bool passed );

bool     test_parse_json     ( char         *test_file    , int         (*expected_value_constructor) (json_value **), result_t expected );
bool     test_serial_json    ( char         *test_file    , char         *expected_file                               , int(*expected_value_constructor) (json_value **), result_t expected );
result_t save_json           ( char         *path         , json_value   *p_value );
bool     value_equals        ( json_value   *a            , json_value   *b );
size_t   load_file           ( const char   *path         , void         *buffer                                      , bool     binary_mode );

void test_g10_g_init ( const char *name );
void test_g10_g_get_active_instance ( const char *name );
void test_g10_user_code ( const char *name );
void test_g10_linear_vectors ( const char *name );

bool test_g_init ( char *test_file, int(*expected_g_instance_constructor) (g_instance **), result_t expected );
bool test_g_get_active_instance ( char *test_file, result_t expected );

// Constructors
int construct_minimal_g10_instance ( g_instance **pp_instance );

// Functions used by the tester
// TODO: Extract declaration and 
int user_code_callback_function ( g_instance *p_instance ) {

    strncpy(p_instance->_name, "G10 tester", 11);

    p_instance->_name[11] = '\0';

    // Success
    return 1;
}

// Entry point
int main ( int argc, const char* argv[] )
{
    
    // Supress compiler warnings
    (void)argc;
    (void)argv;

    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Initialize the timer library
    timer_init();
    log_init(0, false);

    // Formatting
    printf(
        "╭────────────╮\n"\
        "│ g10 tester │\n"\
        "╰────────────╯\n\n"
    );
    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    log_info("\ng10 tests took ");
    print_time_pretty ( (double) ( t1 - t0 ) / (double) timer_seconds_divisor() );
    log_info(" to test\n");

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void print_time_pretty ( double seconds )
{

    // Initialized data
    double _seconds = seconds;
    size_t days = 0,
           hours = 0,
           minutes = 0,
           __seconds = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) log_info("%zu D, ", days);
    
    // Print hours
    if ( hours ) log_info("%zu h, ", hours);

    // Print minutes
    if ( minutes ) log_info("%zu m, ", minutes);

    // Print seconds
    if ( __seconds ) log_info("%zu s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds ) log_info("%zu ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds ) log_info("%zu us", microseconds);
    
    // Done
    return;
}

void run_tests ( void )
{

    // Initialized data
    timestamp g10_core_t0      = 0,
              g10_core_t1      = 0,
              g10_linear_t0    = 0,
              g10_linear_t1    = 0,
              g10_user_code_t0 = 0,
              g10_user_code_t1 = 0;

    ///////////////////
    // Test the core //
    ///////////////////

    // Start timing core 
    g10_core_t0 = timer_high_precision();

        // Test g_init
        test_g10_g_init("g10 core g_init");

        // Test g_get_active_instance
        test_g10_g_get_active_instance("g10 core g_get_active_instance");

    // Stop timing core
    g10_core_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 core took ");
    print_time_pretty ( (double)(g10_core_t1-g10_core_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Start timing linear algebra code
    g10_linear_t0 = timer_high_precision();

        // Test linear
        test_g10_linear_vectors("g10 linear");

    // Stop timing user code
    g10_linear_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear algebra code took ");
    print_time_pretty ( (double)(g10_linear_t1-g10_linear_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Start timing user code
    g10_user_code_t0 = timer_high_precision();

        // Test user_code
        test_g10_user_code("g10 usercode");

    // Stop timing user code
    g10_user_code_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 user code took ");
    print_time_pretty ( (double)(g10_user_code_t1-g10_user_code_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // Done
    return;
}

bool test_g_init ( char *test_file, int(*expected_g_instance_constructor) (g_instance **), result_t expected )
{
    
    // Initialized data
    result_t result = 0;
    g_instance *p_expected_instance = 0,
               *p_return_instance = 0;

    // Construct the expected json value
    if (expected_g_instance_constructor) expected_g_instance_constructor(&p_expected_instance);

    // Parse the instance json
    result = g_init( &p_return_instance, test_file );
    
    // Null pointer match
    if ( p_expected_instance == p_return_instance )
    {
        result=match;
        goto done;
    }

    if ( p_return_instance == (void *) 0 )
    {
        result = zero;
        goto done;
    }

    // Name match
    if ( strcmp(p_expected_instance->_name, p_return_instance->_name) == 0 ) result = match;

    done:

    // Free the instance value
    if (p_return_instance)
        g_exit(&p_return_instance);
    if(p_expected_instance)
        g_exit(&p_expected_instance);

    // Success
    return (result == expected);
}

bool test_g_get_active_instance ( char *test_file, result_t expected )
{

    // Initialized data
    result_t result = 0;
    g_instance *p_return_instance = 0;

    // Parse the instance json
    g_init( &p_return_instance, test_file );

    result = (result_t) g_get_active_instance();

    // Free the instance value
    g_exit(&p_return_instance);
    
    result = (result) ? not_null : zero;

    // Success
    return (result == expected);
}

bool test_vec2_add ( vec2 a, vec2 b, vec2 expected_vec, result_t expected_result )
{

    vec2 result_vec = (vec2) { 0 };
    
    vec2_add_vec2(&result_vec, a, b);

    if (
        ( (int)(expected_vec.x) & ~1) == ((int)(result_vec.x) & ~1) &&
        ( (int)(expected_vec.y) & ~1) == ((int)(result_vec.y) & ~1)
    )
        return (expected_result == match);

    return (expected_result == zero);
}

bool test_vec2_sub ( vec2 a, vec2 b, vec2 expected_vec, result_t expected_result )
{

    vec2 result_vec = (vec2) { 0 };
    
    vec2_sub_vec2(&result_vec, a, b);

    if (
        ( (int)(expected_vec.x) & ~1) == ((int)(result_vec.x) & ~1) &&
        ( (int)(expected_vec.y) & ~1) == ((int)(result_vec.y) & ~1)
    )
        return (expected_result == match);

    return (expected_result == zero);
}

bool test_vec2_mul ( vec2 a, vec2 b, vec2 expected_vec, result_t expected_result )
{

    vec2 result_vec = (vec2) { 0 };
    
    vec2_mul_vec2(&result_vec, a, b);

    if (
        ( (int)(expected_vec.x) & ~1) == ((int)(result_vec.x) & ~1) &&
        ( (int)(expected_vec.y) & ~1) == ((int)(result_vec.y) & ~1)
    )
        return (expected_result == match);

    return (expected_result == zero);
}

bool test_vec2_div ( vec2 a, vec2 b, vec2 expected_vec, result_t expected_result )
{

    vec2 result_vec = (vec2) { 0 };
    
    vec2_div_vec2(&result_vec, a, b);

    if (
        ( (int)(expected_vec.x) & ~1) == ((int)(result_vec.x) & ~1) &&
        ( (int)(expected_vec.y) & ~1) == ((int)(result_vec.y) & ~1)
    )
        return (expected_result == match);

    return (expected_result == zero);
}

bool test_vec3_add ( vec3 a, vec3 b, vec3 expected_vec, result_t expected_result )
{

    vec3 result_vec = (vec3) { 0 };
    
    vec3_add_vec3(&result_vec, a, b);

    if (
        ( (int)(expected_vec.x) & ~1) == ((int)(result_vec.x) & ~1) &&
        ( (int)(expected_vec.y) & ~1) == ((int)(result_vec.y) & ~1) &&
        ( (int)(expected_vec.z) & ~1) == ((int)(result_vec.z) & ~1) 
    )
        return (expected_result == match);

    return (expected_result == zero);
}

bool test_vec3_sub ( vec3 a, vec3 b, vec3 expected_vec, result_t expected_result )
{

    vec3 result_vec = (vec3) { 0 };
    
    vec3_sub_vec3(&result_vec, a, b);

    if (
        ( (int)(expected_vec.x) & ~1) == ((int)(result_vec.x) & ~1) &&
        ( (int)(expected_vec.y) & ~1) == ((int)(result_vec.y) & ~1) &&
        ( (int)(expected_vec.z) & ~1) == ((int)(result_vec.z) & ~1)
    )
        return (expected_result == match);

    return (expected_result == zero);
}

bool test_vec3_mul ( vec3 a, vec3 b, vec3 expected_vec, result_t expected_result )
{

    vec3 result_vec = (vec3) { 0 };
    
    vec3_mul_vec3(&result_vec, a, b);

    if (
        ( (int)(expected_vec.x) & ~1) == ((int)(result_vec.x) & ~1) &&
        ( (int)(expected_vec.y) & ~1) == ((int)(result_vec.y) & ~1) &&
        ( (int)(expected_vec.z) & ~1) == ((int)(result_vec.z) & ~1)
    )
        return (expected_result == match);

    return (expected_result == zero);
}

bool test_vec3_div ( vec3 a, vec3 b, vec3 expected_vec, result_t expected_result )
{

    vec3 result_vec = (vec3) { 0 };
    
    vec3_div_vec3(&result_vec, a, b);

    if (
        ( (int)(expected_vec.x) & ~1) == ((int)(result_vec.x) & ~1) &&
        ( (int)(expected_vec.y) & ~1) == ((int)(result_vec.y) & ~1) &&
        ( (int)(expected_vec.z) & ~1) == ((int)(result_vec.z) & ~1)
    )
        return (expected_result == match);

    return (expected_result == zero);
}

bool test_vec3_dot ( vec3 a, vec3 b, float expected_magnitude, result_t expected_result )
{
    
    float result = dot_product_vec3(a, b);

    if ( ((int)(expected_magnitude) & ~1) == ((int)(result) & ~1) )
        return (expected_result == match);

    return (expected_result == not_equal);
}

bool test_user_code_callback_set ( fn_user_code_callback pfn_user_code_callback, result_t expected )
{

    result_t result = zero;
    g_instance *p_return_instance = 0;

    // Parse the instance json
    g_init( &p_return_instance, "test cases/core/minimal_instance.json" );

    // Set the user code callback
    result = user_code_callback_set(p_return_instance, pfn_user_code_callback);

    // Test for error
    if ( result == zero ) goto done;

    // Test for equality
    if ( pfn_user_code_callback == p_return_instance->context.pfn_user_code_callback )
        result = match;

    done:
    // Free the instance value
    g_exit(&p_return_instance);

    return (result == expected);
}

bool test_user_code_callback ( const char *const path, fn_user_code_callback pfn_user_code_callback, const char *const new_instance_name, result_t expected )
{

    result_t result = zero;
    g_instance *p_return_instance = 0;

    // Parse the instance json
    g_init(&p_return_instance, path);

    // Set the user code callback
    user_code_callback_set(p_return_instance, pfn_user_code_callback);

    // Run the callback
    result = user_code_callback(p_return_instance);

    // Test for error
    if ( result == zero ) goto done;

    // Test for equality
    if ( strcmp(p_return_instance->_name, new_instance_name) == 0 ) result = match;

    done:
    // Free the instance value
    g_exit(&p_return_instance);

    return (result == expected);
}

void test_g10_g_init ( const char *name )
{
    
    // Formatting
    log_info("Scenario: %s\n", name);

    // Test null values
    print_test(name, "null"        , test_g_init(0, (void *) 0, match));
    print_test(name, "empty"       , test_g_init("test cases/core/empty.json"       , (void *) 0, match));
    print_test(name, "empty number", test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "empty object", test_g_init("test cases/core/empty_object.json", (void *) 0, match));

    // Test the name property
    print_test(name, "name too long"  , test_g_init("test cases/core/instance_name_too_long.json"  , (void *) 0, match));
    print_test(name, "name too short" , test_g_init("test cases/core/instance_name_too_short.json" , (void *) 0, match));
    print_test(name, "name wrong type", test_g_init("test cases/core/instance_name_wrong_type.json", (void *) 0, match));
    
    // Test the minimal instance
    print_test(name, "minimal", test_g_init("test cases/core/minimal_instance.json", construct_minimal_g10_instance, match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_g_get_active_instance ( const char *name )
{
    
    // Formatting
    log_info("Scenario: %s\n", name);

    // Test an empty file
    print_test(name, "before_ginit", test_g_get_active_instance(0, zero));
    print_test(name, "after_ginit", test_g_get_active_instance("test cases/core/minimal_instance.json", not_null));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_linear_vec2 ( const char *name )
{
    
    // Formatting
    log_info("Scenario: %s\n", name);

    // Accumulate
    print_test(name, "<1.23, 2.46> + <3.69, 2.46>"  , test_vec2_add((vec2){ 1.23f, 2.46f }, (vec2){ 3.69f, 2.46f }  , (vec2){ 4.92f, 4.92f }, match));
    print_test(name, "<3.21, 6.54> + <-1.23, -3.21>", test_vec2_add((vec2){ 3.21f, 6.54f }, (vec2){ -1.23f, -3.21f }, (vec2){ 1.98f, 3.33f }, match));
    print_test(name, "<0, 0>       + <0, 0>\n"      , test_vec2_add((vec2){ 0.f, 0.f }    , (vec2){ 0.f, 0.f }      , (vec2){ 0.f, 0.f }    , match));

    // Difference
    print_test(name, "<1.23, 2.46> - < 3.69,  2.46>", test_vec2_sub((vec2){ 1.23f, 2.46f }, (vec2){ 3.69f, 2.46f }  , (vec2){ -2.46f, 0.f } , match));
    print_test(name, "<3.21, 6.54> - <-1.23, -3.21>", test_vec2_sub((vec2){ 3.21f, 6.54f }, (vec2){ -1.23f, -3.21f }, (vec2){ 4.44f, 9.75f }, match));
    print_test(name, "<0, 0>       - <0, 0>\n"      , test_vec2_sub((vec2){ 0.f, 0.f }    , (vec2){ 0.f, 0.f }      , (vec2){ 0.f, 0.f }    , match));

    // Product
    print_test(name, "<1.23, 2.46> × < 3.69,  2.46>", test_vec2_mul((vec2){ 1.23f, 2.46f }, (vec2){ 3.69f, 2.46f }  , (vec2){ 4.5387f, 6.0516f }  , match));
    print_test(name, "<3.21, 3.14> × <-1.23, -3.14>", test_vec2_mul((vec2){ 3.21f, 3.14f }, (vec2){ -1.23f, -3.14f }, (vec2){ -3.9483f, -9.8596f }, match));
    print_test(name, "<0, 0>       × <0, 0>\n"      , test_vec2_mul((vec2){ 0.f, 0.f }    , (vec2){ 0.f, 0.f }      , (vec2){ 0.f, 0.f }          , match));

    // Quotient
    print_test(name, "<9.99, 3.33> ÷ <3.33, 1.11>"  , test_vec2_div((vec2){ 9.99f, 3.33f }, (vec2){ 3.33f, 1.11f }  , (vec2){ 3.f, 3.f }     , match));
    print_test(name, "<17.38, 4.2> ÷ <2, 2>"        , test_vec2_div((vec2){ 17.38f, 4.2f }, (vec2){ -2.f, -2.f }    , (vec2){ -8.69f, -2.2f }, match));
    
    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_linear_vec3 ( const char *name )
{
    
    // Formatting
    log_info("Scenario: %s\n", name);

    // Accumulate 
    print_test(name, "<1.1, 2.2, 3.3> + <1.1 ,  2.2, 3.3>" , test_vec3_add((vec3){ 1.1f, 2.2f, 3.3f }, (vec3){  1.1f,  2.2f,  3.3f }, (vec3){  2.2f, 4.4f, 6.6f }, match));
    print_test(name, "<1.1, 2.2, 3.3> + <-3.3, -2.2, -1.1>", test_vec3_add((vec3){ 1.1f, 2.2f, 3.3f }, (vec3){ -3.3f, -2.2f, -1.1f }, (vec3){ -2.2f, 0.f , 2.2f }, match));
    print_test(name, "<0, 0, 0>       + <0,  0,  0>\n"     , test_vec3_add((vec3){ 0.f , 0.f , 0.f  }, (vec3){   0.f,   0.f,  0.f  }, (vec3){  0.f , 0.f , 0.f  }, match));
    
    // Difference
    //print_test(name, "<1.23, 2.46> - <3.69, 2.46>"  , test_vec3_sub((vec2){ 1.23f, 2.46f }, (vec2){ 3.69f, 2.46f }  , (vec2){ -2.46f, 0.f } , match));
    //print_test(name, "<3.21, 6.54> - <-1.23, -3.21>", test_vec3_sub((vec2){ 3.21f, 6.54f }, (vec2){ -1.23f, -3.21f }, (vec2){ 4.44f, 9.75f }, match));
    //print_test(name, "<0, 0>       - <0, 0>\n"      , test_vec3_sub((vec2){ 0.f, 0.f }    , (vec2){ 0.f, 0.f }      , (vec2){ 0.f, 0.f }    , match));
    
    // Product
    //print_test(name, "<1.23, 2.46> × <3.69, 2.46>"  , test_vec3_mul((vec2){ 1.23f, 2.46f }, (vec2){ 3.69f, 2.46f }  , (vec2){ 4.5387f, 6.0516f }  , match));
    //print_test(name, "<3.21, 3.14> × <-1.23, -3.14>", test_vec3_mul((vec2){ 3.21f, 3.14f }, (vec2){ -1.23f, -3.14f }, (vec2){ -3.9483f, -9.8596f }, match));
    //print_test(name, "<0, 0>       × <0, 0>\n"      , test_vec3_mul((vec2){ 0.f, 0.f }    , (vec2){ 0.f, 0.f }      , (vec2){ 0.f, 0.f }          , match));

    // Quotient
    print_test(name, "<8.88, 4.44, 2.22> ÷ <1.11, 2.22, 4.44>\n"  , test_vec3_div((vec3){ 8.88f, 4.44f, 2.22f }, (vec3){ 1.11f, 2.22f, 4.44f }, (vec3){ 8.f, 2.f, 0.5f }     , match));
    //print_test(name, "<17.38, 4.2> ÷ <2, 2>"        , test_vec3_div((vec2){ 17.38f, 4.2f }, (vec2){ -2.f, -2.f }    , (vec2){ -8.69f, -2.2f }, match));

    // Dot product
    print_test(name, "<1,2,3> ⋅ <3,2,1>", test_vec3_dot((vec3){1.f, 2.f, 3.f},(vec3){3.f, 2.f, 1.f},10.f,match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_linear_vectors ( const char *name )
{
    
    // Initialized data
    timestamp g10_vec2_t0 = 0,
              g10_vec2_t1 = 0,
              g10_vec3_t0 = 0,
              g10_vec3_t1 = 0;

    ///////////////
    // Test vec2 //
    ///////////////

    // Start timing core 
    g10_vec2_t0 = timer_high_precision();

        // Test g_init
        test_g10_linear_vec2("g10 linear vec2");

    // Stop timing core
    g10_vec2_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear vec2 took ");
    print_time_pretty ( (double)(g10_vec2_t1-g10_vec2_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");
    
    ///////////////
    // Test vec2 //
    ///////////////

    // Start timing core 
    g10_vec3_t0 = timer_high_precision();

        // Test g_init
        test_g10_linear_vec3("g10 linear vec3");

    // Stop timing core
    g10_vec3_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear vec3 took ");
    print_time_pretty ( (double)(g10_vec3_t1-g10_vec3_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n");

    // TODO: Test 4D vectors

    // Success
    return;
}

void test_g10_linear_matrices ( const char *name )
{
    // TODO: Test 2x2 matrix
    // TODO: Test 3x3 matrix
    // TODO: Test 4x4 matrix

    // Success
    return;
}

void test_g10_quaternion ( const char *name )
{

    // Done
    return;
}

void test_g10_user_code ( const char *name )
{
    
    // Formatting
    log_info("Scenario: %s\n", name);

    // Test callback setter
    print_test(name, "set_null_callback", test_user_code_callback_set(0, zero));
    print_test(name, "set_callback", test_user_code_callback_set(user_code_callback_function, match));

    // Test callback
    print_test(name, "run_callback_null_instance", test_user_code_callback(0, user_code_callback_function, (void *)0, zero));
    print_test(name, "run_callback", test_user_code_callback("test cases/core/minimal_instance.json", user_code_callback_function, "G10 tester", match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

int construct_minimal_g10_instance ( g_instance **pp_instance ) {
    
    g_instance *p_instance = G10_REALLOC(0, sizeof(g_instance));

    *p_instance = (g_instance) {
        ._name = "g10 instance",
        .running = false
    };

    *pp_instance = p_instance;

    return 1;
}

void print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    if ( passed )
        log_pass("%s %s\n",scenario_name, test_name);
    else 
        log_fail("%s %s\n", scenario_name, test_name);
    
    // Increment the pass/fail counter
    if (passed)
        ephemeral_passes++;
    else
        ephemeral_fails++;

    // Increment the test counter
    ephemeral_tests++;

    // Done
    return;
}

void print_final_summary ( void )
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // Print
    log_info("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    log_info("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
    
    // Clear test counters for this test
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Done
    return;
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument checking 
    if ( path == 0 ) goto no_path;

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) 
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG

                #endif

            // Error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG

                #endif

            // Error
            return 0;
        }
    }
}
