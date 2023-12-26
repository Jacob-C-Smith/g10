/** !
 * g10 tester
 * 
 * @file g10_test.c
 * @author Jacob Smith
 */

// Header
#include <sync/sync.h>
#include <log/log.h>
#include <g10/g10.h>
#include <g10/linear.h>
#include <g10/quaternion.h>
#include <g10/transform.h>
#include <g10/user_code.h>

// Enumeration definitions
enum result_e {
    zero     = 0,
    one      = 1,
    match    = 2,
    not_null = 3
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

bool transform_equals_transform ( transform *p_a, transform *p_b );

void test_g10_g_init ( const char *name );
void test_g10_g_get_active_instance ( const char *name );
void test_g10_user_code ( const char *name );
void test_g10_linear_vectors ( const char *name );
void test_g10_linear_matrices ( const char *name );
void test_g10_transform ( const char *name );
void test_g10_quaternion ( const char *name );

bool test_g_init ( char *test_file, int(*expected_g_instance_constructor) (g_instance **), result_t expected );
bool test_g_get_active_instance ( char *test_file, result_t expected );

// Constructors
int construct_minimal_g10_instance ( g_instance **pp_instance );
int construct_identity_transform ( transform **pp_transform );

int get_mat4_from_list ( mat4 *p_mat4 );

// Functions used by the tester
int user_code_callback_function ( g_instance *p_instance )
{

    // Set a new instance name
    strncpy(p_instance->_name, "G10 tester", 11);

    // Store a null terminator
    p_instance->_name[11] = '\0';

    // Success
    return 1;
}

// Entry point
int main ( int argc, const char* argv[] )
{
    
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
    log_info("g10 tests took ");
    print_time_pretty ( (double) ( t1 - t0 ) / (double) timer_seconds_divisor() );
    log_info(" to test\n\n");

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
    timestamp g10_core_t0       = 0,
              g10_core_t1       = 0,
              g10_linear_t0     = 0,
              g10_linear_t1     = 0,
              g10_quaternion_t0 = 0,
              g10_quaternion_t1 = 0,
              g10_transform_t0  = 0,
              g10_transform_t1  = 0,
              g10_user_code_t0  = 0,
              g10_user_code_t1  = 0;

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
    log_info(" to test\n\n");

    /////////////////////////
    // Test linear algebra //
    /////////////////////////

    // Start timing linear algebra code
    g10_linear_t0 = timer_high_precision();

        // Test vectors
        test_g10_linear_vectors("g10 vectors");
        
        // Test matricies
        test_g10_linear_matrices("g10 matricies");

    // Stop timing linear algebra code
    g10_linear_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear algebra took ");
    print_time_pretty ( (double)(g10_linear_t1-g10_linear_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");

    /////////////////////
    // Test quaternion //
    /////////////////////

    // Start timing quaternion code
    g10_quaternion_t0 = timer_high_precision();

        // Test quaternion
        test_g10_quaternion("g10 quaternion");

    // Stop timing quaternion code
    g10_quaternion_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 quaternion took ");
    print_time_pretty ( (double)(g10_quaternion_t1-g10_quaternion_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");

    ////////////////////
    // Test transform //
    ////////////////////

    // Start timing transform code
    g10_transform_t0 = timer_high_precision();

        // Test transform
        test_g10_transform("g10 transform");

    // Stop timing transform code
    g10_transform_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 transform took ");
    print_time_pretty ( (double)(g10_transform_t1-g10_transform_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");

    ////////////////////
    // Test user code //
    ////////////////////

    // Start timing user code
    g10_user_code_t0 = timer_high_precision();

        // Test user_code
        test_g10_user_code("g10 usercode");

    // Stop timing user code
    g10_user_code_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 user code took ");
    print_time_pretty ( (double)(g10_user_code_t1-g10_user_code_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");

    // Done
    return;
}

bool test_g_init ( char *test_file, int(*expected_g_instance_constructor) (g_instance **), result_t expected )
{
    
    // Initialized data
    bool ret = true;
    size_t file_len = 0;
    result_t result = 0,
             value_eq = 0;
    char *file_buf = 0;
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

bool test_transform_from_json ( char *test_file, int(*expected_transform_constructor) (transform **), result_t expected )
{
    result_t result = zero;
    transform *p_transform_result = (void *) 0;
    transform *p_transform_expected = (void *) 0;

    char _buf[4096] = { 0 };
    json_value *p_value = 0;
    g_load_file(test_file, _buf, false);
    transform_from_json(&p_transform_result, p_value);
    if ( expected_transform_constructor )
        expected_transform_constructor(&p_transform_expected);

    result = (transform_equals_transform(p_transform_result, p_transform_expected)) ? match : zero;

    // Success
    return (result == expected);
}

bool test_g_get_active_instance ( char *test_file, result_t expected )
{

    // Initialized data
    result_t result = 0,
             value_eq = 0;
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

bool vec2_equals_vec2 ( vec2 a, vec2 b )
{

    // -0 == 0 
    a.x = (a.x == -0.f) ? 0.f : a.x;
    a.y = (a.y == -0.f) ? 0.f : a.y;

    b.x = (b.x == -0.f) ? 0.f : b.x;
    b.y = (b.y == -0.f) ? 0.f : b.y;

    return 
    (
        ( (*(unsigned long*)&a.x) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.x) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.y) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.y) & 0xFFFFFFF8)
    );
}

bool vec3_equals_vec3 ( vec3 a, vec3 b )
{

    // -0 == 0 
    a.x = (a.x == -0.f) ? 0.f : a.x;
    a.y = (a.y == -0.f) ? 0.f : a.y;
    a.z = (a.z == -0.f) ? 0.f : a.z;

    b.x = (b.x == -0.f) ? 0.f : b.x;
    b.y = (b.y == -0.f) ? 0.f : b.y;
    b.z = (b.z == -0.f) ? 0.f : b.z;
    
    return 
    (
        ( (*(unsigned long*)&a.x) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.x) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.y) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.y) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.z) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.z) & 0xFFFFFFF8)
    );
}

bool vec4_equals_vec4 ( vec4 a, vec4 b )
{

    // -0 == 0 
    a.x = (a.x == -0.f) ? 0.f : a.x;
    a.y = (a.y == -0.f) ? 0.f : a.y;
    a.z = (a.z == -0.f) ? 0.f : a.z;
    a.w = (a.w == -0.f) ? 0.f : a.w;

    b.x = (b.x == -0.f) ? 0.f : b.x;
    b.y = (b.y == -0.f) ? 0.f : b.y;
    b.z = (b.z == -0.f) ? 0.f : b.z;
    b.w = (b.w == -0.f) ? 0.f : b.w;

    return 
    (
        ( (*(unsigned long*)&a.x) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.x) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.y) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.y) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.z) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.z) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.w) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.w) & 0xFFFFFFF8)
    );
}

bool mat2_equals_mat2 ( mat2 m, mat2 n )
{

    // -0 == 0 
    m.a = (m.a == -0.f) ? 0.f : m.a;
    m.b = (m.b == -0.f) ? 0.f : m.b;
    m.c = (m.c == -0.f) ? 0.f : m.c;
    m.d = (m.d == -0.f) ? 0.f : m.d;

    n.a = (n.a == -0.f) ? 0.f : n.a;
    n.b = (n.b == -0.f) ? 0.f : n.b;
    n.c = (n.c == -0.f) ? 0.f : n.c;
    n.d = (n.d == -0.f) ? 0.f : n.d;

    return 
    (
        ( (*(unsigned long*)&m.a) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.a) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.b) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.b) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.c) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.c) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.d) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.d) & 0xFFFFFFF8) 
    );
}

bool mat3_equals_mat3 ( mat3 m, mat3 n )
{

    // -0 == 0 
    m.a = (m.a == -0.f) ? 0.f : m.a;
    m.b = (m.b == -0.f) ? 0.f : m.b;
    m.c = (m.c == -0.f) ? 0.f : m.c;
    m.d = (m.d == -0.f) ? 0.f : m.d;
    m.e = (m.e == -0.f) ? 0.f : m.e;
    m.f = (m.f == -0.f) ? 0.f : m.f;
    m.g = (m.g == -0.f) ? 0.f : m.g;
    m.h = (m.h == -0.f) ? 0.f : m.h;
    m.i = (m.i == -0.f) ? 0.f : m.i;

    n.a = (n.a == -0.f) ? 0.f : n.a;
    n.b = (n.b == -0.f) ? 0.f : n.b;
    n.c = (n.c == -0.f) ? 0.f : n.c;
    n.d = (n.d == -0.f) ? 0.f : n.d;
    n.e = (n.e == -0.f) ? 0.f : n.e;
    n.f = (n.f == -0.f) ? 0.f : n.f;
    n.g = (n.g == -0.f) ? 0.f : n.g;
    n.h = (n.h == -0.f) ? 0.f : n.h;
    n.i = (n.i == -0.f) ? 0.f : n.i;

    return 
    (
        ( (*(unsigned long*)&m.a) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.a) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.b) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.b) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.c) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.c) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.d) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.d) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.e) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.e) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.f) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.f) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.g) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.g) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.h) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.h) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.i) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.i) & 0xFFFFFFF8) 
    );
}

bool mat4_equals_mat4 ( mat4 m, mat4 n )
{

    // -0 == 0 
    m.a = (m.a == -0.f) ? 0.f : m.a;
    m.b = (m.b == -0.f) ? 0.f : m.b;
    m.c = (m.c == -0.f) ? 0.f : m.c;
    m.d = (m.d == -0.f) ? 0.f : m.d;
    m.e = (m.e == -0.f) ? 0.f : m.e;
    m.f = (m.f == -0.f) ? 0.f : m.f;
    m.g = (m.g == -0.f) ? 0.f : m.g;
    m.h = (m.h == -0.f) ? 0.f : m.h;
    m.i = (m.i == -0.f) ? 0.f : m.i;
    m.j = (m.j == -0.f) ? 0.f : m.j;
    m.k = (m.k == -0.f) ? 0.f : m.k;
    m.l = (m.l == -0.f) ? 0.f : m.l;
    m.m = (m.m == -0.f) ? 0.f : m.m;
    m.n = (m.n == -0.f) ? 0.f : m.n;
    m.o = (m.o == -0.f) ? 0.f : m.o;
    m.p = (m.p == -0.f) ? 0.f : m.p;

    n.a = (n.a == -0.f) ? 0.f : n.a;
    n.b = (n.b == -0.f) ? 0.f : n.b;
    n.c = (n.c == -0.f) ? 0.f : n.c;
    n.d = (n.d == -0.f) ? 0.f : n.d;
    n.e = (n.e == -0.f) ? 0.f : n.e;
    n.f = (n.f == -0.f) ? 0.f : n.f;
    n.g = (n.g == -0.f) ? 0.f : n.g;
    n.h = (n.h == -0.f) ? 0.f : n.h;
    n.i = (n.i == -0.f) ? 0.f : n.i;
    n.j = (n.j == -0.f) ? 0.f : n.j;
    n.k = (n.k == -0.f) ? 0.f : n.k;
    n.l = (n.l == -0.f) ? 0.f : n.l;
    n.m = (n.m == -0.f) ? 0.f : n.m;
    n.n = (n.n == -0.f) ? 0.f : n.n;
    n.o = (n.o == -0.f) ? 0.f : n.o;
    n.p = (n.p == -0.f) ? 0.f : n.p;

    return 
    (
        ( (*(unsigned long*)&m.a) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.a) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.b) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.b) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.c) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.c) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.d) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.d) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.e) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.e) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.f) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.f) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.g) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.g) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.h) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.h) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.i) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.i) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.j) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.j) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.k) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.k) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.l) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.l) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.m) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.m) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.n) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.n) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.o) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.o) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&m.p) & 0xFFFFFFF8) == ( (*(unsigned long*)&n.p) & 0xFFFFFFF8) 
    );
}

bool transform_equals_transform ( transform *p_a, transform *p_b )
{

    if ( p_a == p_b ) return true;
    

    return 
    (
        vec3_equals_vec3(p_a->location, p_b->location) && 
        vec3_equals_vec3(p_a->scale, p_b->scale)     
    );
}

bool test_vec2_add ( vec2 a, vec2 b, vec2 expected_vec, result_t expected_result )
{

    vec2 result_vec = (vec2) { 0 };
    
    vec2_add_vec2(&result_vec, a, b);

    return expected_result == ( vec2_equals_vec2(expected_vec, result_vec) ? match : zero); 
}

bool test_vec2_sub ( vec2 a, vec2 b, vec2 expected_vec, result_t expected_result )
{

    vec2 result_vec = (vec2) { 0 };
    
    vec2_sub_vec2(&result_vec, a, b);

    return expected_result == ( vec2_equals_vec2(expected_vec, result_vec) ? match : zero); 
}

bool test_vec2_mul ( vec2 a, vec2 b, vec2 expected_vec, result_t expected_result )
{

    vec2 result_vec = (vec2) { 0 };
    
    vec2_mul_vec2(&result_vec, a, b);

    return expected_result == ( vec2_equals_vec2(expected_vec, result_vec) ? match : zero); 
}

bool test_vec2_div ( vec2 a, vec2 b, vec2 expected_vec, result_t expected_result )
{

    vec2 result_vec = (vec2) { 0 };
    
    vec2_div_vec2(&result_vec, a, b);

    return expected_result == ( vec2_equals_vec2(expected_vec, result_vec) ? match : zero); 
}

bool test_vec2_length ( vec2 v, float expected_length, result_t expected_result )
{
    float result = 0.f;
    
    vec2_length(&result, v);

    return expected_result == ( ( (*(unsigned long*)&result) & 0xFFFFFFF8) == ( (*(unsigned long*)&expected_length) & 0xFFFFFFF8) ? match : zero); 
}

bool test_vec2_promote_vec3 ( vec2 v, vec3 expected_vec, result_t expected_result )
{
    vec3 result_vec = { 0 };

    vec2_to_vec3(&result_vec, v);

    return expected_result == ( vec3_equals_vec3(expected_vec, result_vec) ? match : zero); 
}

bool test_vec2_promote_vec4 ( vec2 v, vec4 expected_vec, result_t expected_result )
{
    vec4 result_vec = { 0 };

    vec2_to_vec4(&result_vec, v);

    return expected_result == ( vec4_equals_vec4(expected_vec, result_vec) ? match : zero); 
}

bool test_vec3_add ( vec3 a, vec3 b, vec3 expected_vec, result_t expected_result )
{

    vec3 result_vec = (vec3) { 0 };
    
    vec3_add_vec3(&result_vec, a, b);

    return expected_result == ( vec3_equals_vec3(expected_vec, result_vec) ? match : zero); 
}

bool test_vec3_sub ( vec3 a, vec3 b, vec3 expected_vec, result_t expected_result )
{

    vec3 result_vec = (vec3) { 0 };
    
    vec3_sub_vec3(&result_vec, a, b);

    return expected_result == ( vec3_equals_vec3(expected_vec, result_vec) ? match : zero); 
}

bool test_vec3_mul ( vec3 a, vec3 b, vec3 expected_vec, result_t expected_result )
{

    vec3 result_vec = (vec3) { 0 };
    
    vec3_mul_vec3(&result_vec, a, b);

    return expected_result == ( vec3_equals_vec3(expected_vec, result_vec) ? match : zero); 
}

bool test_vec3_div ( vec3 a, vec3 b, vec3 expected_vec, result_t expected_result )
{

    vec3 result_vec = (vec3) { 0 };
    
    vec3_div_vec3(&result_vec, a, b);

    return expected_result == ( vec3_equals_vec3(expected_vec, result_vec) ? match : zero); 
}

bool test_vec3_length ( vec3 v, float expected_length, result_t expected_result )
{
    float result = 0.f;
    
    vec3_length(&result, v);

    return expected_result == ( ( (*(unsigned long*)&result) & 0xFFFFFFF8) == ( (*(unsigned long*)&expected_length) & 0xFFFFFFF8) ? match : zero); 
}

bool test_vec3_demote_vec2 ( vec3 v, vec2 expected_vec, result_t expected_result )
{
    vec2 result_vec = { 0 };

    vec3_to_vec2(&result_vec, v);

    return expected_result == ( vec2_equals_vec2(expected_vec, result_vec) ? match : zero); 
}

bool test_vec3_promote_vec4 ( vec3 v, vec4 expected_vec, result_t expected_result )
{
    vec4 result_vec = { 0 };

    vec3_to_vec4(&result_vec, v);

    return expected_result == ( vec4_equals_vec4(expected_vec, result_vec) ? match : zero); 
}

bool test_vec4_add ( vec4 a, vec4 b, vec4 expected_vec, result_t expected_result )
{

    vec4 result_vec = (vec4) { 0 };
    
    vec4_add_vec4(&result_vec, a, b);

    return expected_result == ( vec4_equals_vec4(expected_vec, result_vec) ? match : zero); 
}

bool test_vec4_sub ( vec4 a, vec4 b, vec4 expected_vec, result_t expected_result )
{

    vec4 result_vec = (vec4) { 0 };
    
    vec4_sub_vec4(&result_vec, a, b);

    return expected_result == ( vec4_equals_vec4(expected_vec, result_vec) ? match : zero); 
}

bool test_vec4_mul ( vec4 a, vec4 b, vec4 expected_vec, result_t expected_result )
{

    vec4 result_vec = (vec4) { 0 };
    
    vec4_mul_vec4(&result_vec, a, b);

    return expected_result == ( vec4_equals_vec4(expected_vec, result_vec) ? match : zero); 
}

bool test_vec4_div ( vec4 a, vec4 b, vec4 expected_vec, result_t expected_result )
{

    vec4 result_vec = (vec4) { 0 };
    
    vec4_div_vec4(&result_vec, a, b);

    return expected_result == ( vec4_equals_vec4(expected_vec, result_vec) ? match : zero); 
}

bool test_vec4_length ( vec4 v, float expected_length, result_t expected_result )
{
    float result = 0.f;
    
    vec4_length(&result, v);

    return expected_result == ( ( (*(unsigned long*)&result) & 0xFFFFFFF8) == ( (*(unsigned long*)&expected_length) & 0xFFFFFFF8) ? match : zero); 
}

bool test_vec4_demote_vec2 ( vec4 v, vec2 expected_vec, result_t expected_result )
{
    vec2 result_vec = { 0 };

    vec4_to_vec2(&result_vec, v);

    return expected_result == ( vec2_equals_vec2(expected_vec, result_vec) ? match : zero); 
}

bool test_vec4_demote_vec3 ( vec4 v, vec3 expected_vec, result_t expected_result )
{
    vec3 result_vec = { 0 };

    vec4_to_vec3(&result_vec, v);

    return expected_result == ( vec3_equals_vec3(expected_vec, result_vec) ? match : zero); 
}

bool test_mat2_identity ( mat2 expected_mat, result_t expected_result)
{
    mat2 result_mat = { 0 };

    mat2_identity(&result_mat);

    return expected_result == ( mat2_equals_mat2(expected_mat, result_mat) ? match : zero); 
}

bool test_mat2_mul_vec2 ( mat2 m, vec2 v, vec2 expected_vec, result_t expected_result )
{
    vec2 result_vec = { 0 };

    mat2_mul_vec2(&result_vec, m, v);

    return expected_result == ( vec2_equals_vec2(expected_vec, result_vec) ? match : zero); 
}

bool test_mat2_mul_mat2 ( mat2 m, mat2 n, mat2 expected_mat, result_t expected_result )
{
    mat2 result_mat = { 0 };

    mat2_mul_mat2(&result_mat, m, n);

    return expected_result == ( mat2_equals_mat2(expected_mat, result_mat) ? match : zero); 
}

bool test_mat2_transpose ( mat2 m, mat2 expected_mat, result_t expected_result )
{
    mat2 result_mat = { 0 };

    mat2_transpose(&result_mat, m);

    return expected_result == ( mat2_equals_mat2(expected_mat, result_mat) ? match : zero); 
}

bool test_mat2_promote_mat3 ( mat2 m, mat3 expected_mat, result_t expected_result )
{
    mat3 result_mat = { 0 };

    mat2_to_mat3(&result_mat, m);

    return expected_result == ( mat3_equals_mat3(expected_mat, result_mat) ? match : zero); 
}

bool test_mat2_promote_mat4 ( mat2 m, mat4 expected_mat, result_t expected_result )
{
    mat4 result_mat = { 0 };

    mat2_to_mat4(&result_mat, m);

    return expected_result == ( mat4_equals_mat4(expected_mat, result_mat) ? match : zero); 
}

bool test_mat3_identity ( mat3 expected_mat, result_t expected_result)
{
    mat3 result_mat = { 0 };

    mat3_identity(&result_mat);

    return expected_result == ( mat3_equals_mat3(expected_mat, result_mat) ? match : zero); 
}

bool test_mat3_mul_vec3 ( mat3 m, vec3 v, vec3 expected_vec, result_t expected_result )
{
    vec3 result_vec = { 0 };

    mat3_mul_vec3(&result_vec, m, v);

    return expected_result == ( vec3_equals_vec3(expected_vec, result_vec) ? match : zero); 
}

bool test_mat3_mul_mat3 ( mat3 m, mat3 n, mat3 expected_mat, result_t expected_result )
{
    mat3 result_mat = { 0 };

    mat3_mul_mat3(&result_mat, m, n);

    return expected_result == ( mat3_equals_mat3(expected_mat, result_mat) ? match : zero); 
}

bool test_mat3_transpose ( mat3 m, mat3 expected_mat, result_t expected_result )
{
    mat3 result_mat = { 0 };

    mat3_transpose(&result_mat, m);

    return expected_result == ( mat3_equals_mat3(expected_mat, result_mat) ? match : zero); 
}

bool test_mat3_demote_mat2 ( mat3 m, mat2 expected_mat, result_t expected_result )
{
    mat2 result_mat = { 0 };

    mat3_to_mat2(&result_mat, m);

    return expected_result == ( mat2_equals_mat2(expected_mat, result_mat) ? match : zero); 
}

bool test_mat3_promote_mat4 ( mat3 m, mat4 expected_mat, result_t expected_result )
{
    mat4 result_mat = { 0 };

    mat3_to_mat4(&result_mat, m);

    return expected_result == ( mat4_equals_mat4(expected_mat, result_mat) ? match : zero); 
}

bool test_mat4_identity ( mat4 expected_mat, result_t expected_result)
{
    mat4 result_mat = { 0 };

    mat4_identity(&result_mat);

    return expected_result == ( mat4_equals_mat4(expected_mat, result_mat) ? match : zero); 
}

bool test_mat4_demote_mat2 ( mat4 m, mat2 expected_mat, result_t expected_result )
{
    mat2 result_mat = { 0 };

    mat4_to_mat2(&result_mat, m);

    return expected_result == ( mat2_equals_mat2(expected_mat, result_mat) ? match : zero); 
}

bool test_mat4_demote_mat3 ( mat4 m, mat3 expected_mat, result_t expected_result )
{
    mat3 result_mat = { 0 };

    mat4_to_mat3(&result_mat, m);

    return expected_result == ( mat3_equals_mat3(expected_mat, result_mat) ? match : zero); 
}

bool quaternion_equals_quaternion ( quaternion a, quaternion b )
{

    // -0 == 0 
    a.u = (a.u == -0.f) ? 0.f : a.u;
    a.i = (a.i == -0.f) ? 0.f : a.i;
    a.j = (a.j == -0.f) ? 0.f : a.j;
    a.k = (a.k == -0.f) ? 0.f : a.k;

    b.u = (b.u == -0.f) ? 0.f : b.u;
    b.i = (b.i == -0.f) ? 0.f : b.i;
    b.j = (b.j == -0.f) ? 0.f : b.j;
    b.k = (b.k == -0.f) ? 0.f : b.k;

    return 
    (
        ( (*(unsigned long*)&a.u) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.u) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.i) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.i) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.j) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.j) & 0xFFFFFFF8) &&
        ( (*(unsigned long*)&a.k) & 0xFFFFFFF8) == ( (*(unsigned long*)&b.k) & 0xFFFFFFF8)
    );
} 

bool test_quaternion_identity ( quaternion expected_quaternion, result_t expected_result )
{
    quaternion result_quaternion = { 0 };

    quaternion_identity(&result_quaternion);

    return expected_result == ( quaternion_equals_quaternion(expected_quaternion, result_quaternion) ? match : zero); 
} 

bool test_user_code_callback_set ( fn_user_code_callback pfn_user_code_callback, result_t expected )
{

    result_t result = zero;
    g_instance *p_return_instance = 0;

    // Parse the instance json
    g_init( &p_return_instance, "test\ cases/core/minimal_instance.json" );

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
    log_scenario("%s\n", name);

    // Test null values
    print_test(name, "null"            , test_g_init(0, (void *) 0, match));
    print_test(name, "empty"           , test_g_init("test cases/core/empty.json", (void *) 0, match));
    print_test(name, "empty number"    , test_g_init("test cases/core/empty_number.json", (void *) 0, match));
    print_test(name, "empty object"    , test_g_init("test cases/core/empty_object.json", (void *) 0, match));

    // Test the minimal instance
    print_test(name, "minimal", test_g_init("test cases/core/minimal_instance.json", construct_minimal_g10_instance, match));

    // Test the name property
    print_test(name, "name too long", test_g_init("test cases/core/instance_name_too_long.json", (void *) 0, match));
    print_test(name, "name too short", test_g_init("test cases/core/instance_name_too_short.json", (void *) 0, match));
    print_test(name, "name wrong type", test_g_init("test cases/core/instance_name_wrong_type.json", (void *) 0, match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_g_get_active_instance ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

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
    log_scenario("%s\n", name);

    // Accumulate
    print_test(name, "add <1.23, 2.46> + <3.69, 2.46>"  , test_vec2_add((vec2){ 1.23f, 2.46f }, (vec2){ 3.69f, 2.46f }  , (vec2){ 4.92f, 4.92f }, match));
    print_test(name, "add <3.21, 6.54> + <-1.23, -3.21>", test_vec2_add((vec2){ 3.21f, 6.54f }, (vec2){ -1.23f, -3.21f }, (vec2){ 1.98f, 3.33f }, match));
    print_test(name, "add <0, 0>       + <0, 0>\n"      , test_vec2_add((vec2){ 0.f, 0.f }    , (vec2){ 0.f, 0.f }      , (vec2){ 0.f, 0.f }    , match));

    // Difference
    print_test(name, "sub <1.23, 2.46> - <3.69, 2.46>"  , test_vec2_sub((vec2){ 1.23f, 2.46f }, (vec2){ 3.69f, 2.46f }  , (vec2){ -2.46f, 0.f } , match));
    print_test(name, "sub <3.21, 6.54> - <-1.23, -3.21>", test_vec2_sub((vec2){ 3.21f, 6.54f }, (vec2){ -1.23f, -3.21f }, (vec2){ 4.44f, 9.75f }, match));
    print_test(name, "sub <0, 0>       - <0, 0>\n"      , test_vec2_sub((vec2){ 0.f, 0.f }    , (vec2){ 0.f, 0.f }      , (vec2){ 0.f, 0.f }    , match));

    // Product
    print_test(name, "mul <1.23, 2.46> × <3.69, 2.46>"  , test_vec2_mul((vec2){ 1.23f, 2.46f }, (vec2){ 3.69f, 2.46f }  , (vec2){ 4.5387f, 6.0516f }  , match));
    print_test(name, "mul <3.21, 3.14> × <-1.23, -3.14>", test_vec2_mul((vec2){ 3.21f, 3.14f }, (vec2){ -1.23f, -3.14f }, (vec2){ -3.9483f, -9.8596f }, match));
    print_test(name, "mul <0, 0>       × <0, 0>\n"      , test_vec2_mul((vec2){ 0.f, 0.f }    , (vec2){ 0.f, 0.f }      , (vec2){ 0.f, 0.f }          , match));

    // Quotient
    print_test(name, "div <9.99, 3.33> ÷ <3.33, 1.11>"  , test_vec2_div((vec2){ 9.99f, 3.33f }, (vec2){ 3.33f, 1.11f }  , (vec2){ 3.f, 3.f }     , match));
    print_test(name, "div <17.38, 4.2> ÷ <-2, -2>"        , test_vec2_div((vec2){ 17.38f, 4.2f }, (vec2){ -2.f, -2.f }    , (vec2){ -8.69f, -2.1f }, match));
    print_test(name, "div <3.45, 9.0>  ÷ <0.5, 2>\n"      , test_vec2_div((vec2){ 3.45f, 9.0f } , (vec2){ 0.5f, 2.f }     , (vec2){ 6.9f, 4.5f }   , match));
    
    // Length
    print_test(name, "length ||<0.0, 0.0>||"  , test_vec2_length((vec2){ 0.0f, 0.0f }, 0.0f, match));
    print_test(name, "length ||<1.0, 0.0>||"  , test_vec2_length((vec2){ 1.0f, 0.0f }, 1.0f, match));
    print_test(name, "length ||<0.0, 1.0>||"  , test_vec2_length((vec2){ 0.0f, 1.0f }, 1.0f, match));
    print_test(name, "length ||<1.0, 1.0>||\n", test_vec2_length((vec2){ 1.0f, 1.0f } , (M_SQRT1_2 * 2), match));
    
    // Promote 3D
    print_test(name, "to vec3 < 1.0, 2.0> -> <1.0, 2.0, 0.0>", test_vec2_promote_vec3((vec2){1.0f, 2.0f}, (vec3){1.0f, 2.0f, 0.0f}, match));
    print_test(name, "to vec3 < 0.0, 0.0> -> <0.0, 0.0, 0.0>", test_vec2_promote_vec3((vec2){0.0f, 0.0f}, (vec3){0.0f, 0.0f, 0.0f}, match));
    print_test(name, "to vec3 <-6.0, 3.0> -> <-6.0, 3.0, 0.0>\n", test_vec2_promote_vec3((vec2){-6.0f, 3.0f}, (vec3){-6.0f, 3.0f, 0.0f}, match));

    // Promote 4D
    print_test(name, "to vec4 < 1.0, 2.0> -> <1.0, 2.0, 0.0, 0.0>", test_vec2_promote_vec4((vec2){1.0f, 2.0f}, (vec4){1.0f, 2.0f, 0.0f, 0.0f}, match));
    print_test(name, "to vec4 < 0.0, 0.0> -> <0.0, 0.0, 0.0, 0.0>", test_vec2_promote_vec4((vec2){0.0f, 0.0f}, (vec4){0.0f, 0.0f, 0.0f, 0.0f}, match));
    print_test(name, "to vec4 <-6.0, 3.0> -> <-6.0, 3.0, 0.0, 0.0>\n", test_vec2_promote_vec4((vec2){-6.0f, 3.0f}, (vec4){-6.0f, 3.0f, 0.0f, 0.0f}, match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_linear_vec3 ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

    // Accumulate 
    print_test(name, "add < 1.1, 2.2, 3.3 > + < 1.1,  2.2,  3.3>"  , test_vec3_add((vec3){ 1.1f, 2.2f, 3.3f }, (vec3){  1.1f, 2.2f, 3.3f }  , (vec3){ 2.2f, 4.4f, 6.6f }, match));
    print_test(name, "add < 1.1, 2.2, 3.3 > + <-3.3, -2.2, -1.1>"  , test_vec3_add((vec3){ 1.1f, 2.2f, 3.3f }, (vec3){ -3.3f, -2.2f, -1.1f }  , (vec3){ -2.2f, 0.f, 2.2f }, match));
    print_test(name, "add <-3.3, 2.2,-1.1 > + <2.2 , -4.4,  6.6>\n"  , test_vec3_add((vec3){-3.3f, 2.2f,-1.1f }, (vec3){  2.2f, -4.4f, 6.6f }  , (vec3){ -1.1f, -2.2f, 5.5f }, match));

    // Difference
    print_test(name, "sub < 1.1, 2.2, 3.3 > - < 1.1,  2.2,  3.3>"  , test_vec3_sub((vec3){ 1.1f, 2.2f, 3.3f }, (vec3){  1.1f, 2.2f, 3.3f }  , (vec3){ 0.0f, 0.0f, 0.0f }, match));
    print_test(name, "sub < 1.1, 2.2, 3.3 > - <-3.3, -2.2, -1.1>"  , test_vec3_sub((vec3){ 1.1f, 2.2f, 3.3f }, (vec3){ -3.3f, -2.2f, -1.1f }  , (vec3){ 4.4f, 4.4f, 4.4f }, match));
    print_test(name, "sub <-3.3, 2.2,-1.1 > - <2.2 , -4.4,  6.6>\n"  , test_vec3_sub((vec3){-3.3f, 2.2f,-1.1f }, (vec3){  2.2f, -4.4f, 6.6f }  , (vec3){ -5.5f, 6.6f, -7.7f }, match));

    // Product
    print_test(name, "mul <1.23, 4.56, 7.89>  × <9.87, 6.54, 3.21>"  , test_vec3_mul((vec3){ 1.23f, 4.56f, 7.89f }    , (vec3){ 9.87f, 6.54f, 3.21f }      , (vec3){ 12.1401f, 29.8224f, 25.3269f }          , match));
    print_test(name, "mul <3.21, 10.0, 20.50> × <5, 6, 7>", test_vec3_mul((vec3){ 3.21f, 10.0f, 20.50f }    , (vec3){ 5.0f, 6.0f, 7.0f }      , (vec3){ 16.05f, 60.0f, 143.5f }          , match));
    print_test(name, "mul <0, 0, 0>           × <0, 0, 0>\n"      , test_vec3_mul((vec3){ 0.f, 0.f, 0.f }    , (vec3){ 0.f, 0.f, 0.f }      , (vec3){ 0.f, 0.f, 0.f }          , match));

    // Quotient
    print_test(name, "div <-8.88 , 4.44, -2.22 > ÷ <-1.11, 2.22, 4.44>", test_vec3_div((vec3){ 8.88f, 4.44f, 2.22f }, (vec3){ 1.11f, 2.22f, 4.44f }, (vec3){ 8.f, 2.f, 0.5f }     , match));
    print_test(name, "div <4.0  , -6.0 , 5.0   > ÷ <2.0  , 3.0 , 2.5 >", test_vec3_div((vec3){ 4.0f, 6.0f, 5.0f }, (vec3){ 2.0f, 3.0f, 2.5f }, (vec3){ 2.f, 2.f, 2.0f }     , match));
    print_test(name, "div <-18.58, 0.2 , -60.75> ÷ <5.3  , 7.6 , 9.2 >\n", test_vec3_div((vec3){ -18.58f, 0.2f, -60.75f }, (vec3){ 5.3f, 7.6f, 9.2f }    , (vec3){ -3.50566f, 0.02631578f, -6.60326f}, match));

    // Length
    print_test(name, "length ||<0.0, 0.0, 0.0>||"  , test_vec3_length((vec3){ 0.0f, 0.0f, 0.f }, 0.0f, match));
    print_test(name, "length ||<2.0, 0.0, 1.0>||"  , test_vec3_length((vec3){ 2.0f, 0.f, 1.f }, 2.2360679775f, match));
    print_test(name, "length ||<1.0, 1.0, 1.0>||\n", test_vec3_length((vec3){ 1.0f, 1.0f, 1.0f } , 1.7320507f, match));
    
    // Demote 2D
    print_test(name, "to vec2 < 1.0,  2.0, 3.0> -> < 1.0, 2.0>", test_vec3_demote_vec2((vec3){1.0f, 2.0f, 3.0f}, (vec2){1.0f, 2.0f}, match));
    print_test(name, "to vec2 < 0.0,  0.0, 0.0> -> < 0.0, 0.0>", test_vec3_demote_vec2((vec3){0.0f, 0.0f, 0.0f}, (vec2){0.0f, 0.0f}, match));
    print_test(name, "to vec2 <-6.0, -3.0, 2.0> -> <-6.0, -3.0>\n", test_vec3_demote_vec2((vec3){-6.0f, -3.0f, 2.0f}, (vec2){-6.0f, -3.0f}, match));

    // Promote 4D
    print_test(name, "to vec4 < 1.0,  2.0, 3.0> -> < 1.0, 2.0, 3.0, 0.0>", test_vec3_promote_vec4((vec3){1.0f, 2.0f, 3.0f}, (vec4){1.0f, 2.0f, 3.0f, 0.0f}, match));
    print_test(name, "to vec4 < 0.0,  0.0, 0.0> -> < 0.0, 0.0, 0.0, 0.0>", test_vec3_promote_vec4((vec3){0.0f, 0.0f, 0.0f}, (vec4){0.0f, 0.0f, 0.f, 0.f}, match));
    print_test(name, "to vec4 <-6.0, -3.0, 2.0> -> <-6.0, -3.0, 2.0, 0.0>\n", test_vec3_promote_vec4((vec3){-6.0f, -3.0f, 2.0f}, (vec4){-6.0f, -3.0f, 2.0f, 0.0f}, match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_linear_vec4 ( const char *name )
{
    
     // Formatting
    log_scenario("%s\n", name);

    // Accumulate 
    print_test(name, "add <  1.1, 2.2,  3.3, 4.4 > + < 1.1,  2.2,  3.3, 4.4 >"  , test_vec4_add((vec4){ 1.1f, 2.2f, 3.3f, 4.4f }, (vec4){  1.1f,  2.2f,  3.3f,  4.4f }  , (vec4){ 2.2f, 4.4f, 6.6f, 8.8f }, match));
    print_test(name, "add <  1.1, 2.2,  3.3, 4.4 > + <-4.4, -3.3, -2.2, -1.1>"  , test_vec4_add((vec4){ 1.1f, 2.2f, 3.3f, 4.4f }, (vec4){ -4.4f, -3.3f, -2.2f, -1.1f }  , (vec4){ -3.3f, -1.1f, 1.1f, 3.3f }, match));
    print_test(name, "add < -1.1, 2.2, -3.3, 4.4 > + < 5.5, -6.6,  7.7, -8.8>\n"  , test_vec4_add((vec4){ -1.1f, 2.2f, -3.3f, 4.4f }, (vec4){ 5.5f, -6.6f, 7.7f, -8.8f }  , (vec4){ 4.4f, -4.4f, 4.4f, -4.4f }, match));

    // Difference
    print_test(name, "sub <  0.0, 0.0,  0.0, 0.0 > - < 0.0,  0.0, 0.0,  0.0 >" , test_vec4_sub((vec4){ 0.0f, 0.0f, 0.0f, 0.0f }, (vec4){  0.0f, 0.0f, 0.0f, 0.0f }  , (vec4){ 0.0f, 0.0f, 0.0f, 0.0f}, match));
    print_test(name, "sub <  1.1, 2.2,  3.3, 4.4 > - < 1.1,  2.2, 3.3,  4.4>"  , test_vec4_sub((vec4){ 1.1f, 2.2f, 3.3f, 4.4f }, (vec4){ 1.1f, 2.2f, 3.3f, 4.4f }  , (vec4){ 0.0f, 0.0f, 0.0f, 0.0f }, match));
    print_test(name, "sub < -1.1, 2.2, -3.3, 4.4 > - < 5.5, -6.6, 7.7, -8.8>\n", test_vec4_sub((vec4){ -1.1f, 2.2f, -3.3f, 4.4f }, (vec4){ 5.5f, -6.6f, 7.7f, -8.8f }  , (vec4){ -6.6f, 8.8f, -11.0f, 13.2f }, match));

    // Product
    print_test(name, "mul <0, 0, 0, 0>  × <0, 0, 0, 0>"      , test_vec4_mul((vec4){ 0.f, 0.f, 0.f, 0.f }    , (vec4){ 0.f, 0.f, 0.f, 0.f }      , (vec4){ 0.f, 0.f, 0.f, 0.f }          , match));
    print_test(name, "mul <1, 2, 3, 4>  × <4, 3, 2, 1>"      , test_vec4_mul((vec4){ 1.f, 2.f, 3.f, 4.f }    , (vec4){ 4.f, 3.f, 2.f, 1.f }      , (vec4){ 4.0f, 6.0f, 6.0f, 4.0f }          , match));
    print_test(name, "mul <12, 9, 6, 3> × <3, 6, 6, 3>\n"      , test_vec4_mul((vec4){ 12.f, 9.f, 6.f, 3.f }   , (vec4){ 3.f, 6.f, 6.f, 3.f }      , (vec4){ 36.0f, 54.0f, 36.0f, 9.0f }          , match));

    // TODO: Quotient
    //print_test(name, "div <-8.88 , 4.44, -2.22 > ÷ <-1.11, 2.22, 4.44>", test_vec3_div((vec3){ 8.88f, 4.44f, 2.22f }, (vec3){ 1.11f, 2.22f, 4.44f }, (vec3){ 8.f, 2.f, 0.5f }     , match));
    //print_test(name, "div <4.0  , -6.0 , 5.0   > ÷ <2.0  , 3.0 , 2.5 >", test_vec3_div((vec3){ 4.0f, 6.0f, 5.0f }, (vec3){ 2.0f, 3.0f, 2.5f }, (vec3){ 2.f, 2.f, 2.0f }     , match));
    //print_test(name, "div <-18.58, 0.2 , -60.75> ÷ <5.3  , 7.6 , 9.2 >", test_vec3_div((vec2){ 17.38f, 4.2f }, (vec2){ -2.f, -2.f }    , (vec2){ -8.69f, -2.2f }, match));

    // Length
    print_test(name, "length ||<0.0, 0.0, 0.0, 0,0>||"  , test_vec4_length((vec4){ 0.0f, 0.0f, 0.0f, 0.0f }, 0.0f, match));
    print_test(name, "length ||<2.0, 0.0, 1.0, 3.0>||"  , test_vec4_length((vec4){ 2.0f, 0.0f, 1.0f, 3.0f }, 3.74165739f, match));
    print_test(name, "length ||<1.0, 1.0, 1.0, 1.0>||\n", test_vec4_length((vec4){ 1.0f, 1.0f, 1.0f, 1.0f }, 2.0f, match));
    
    // Demote 2D
    print_test(name, "to vec2 < 1.0,  2.0, 3.0, 4.0> -> < 1.0, 2.0>", test_vec4_demote_vec2((vec4){1.0f, 2.0f, 3.0f, 4.0f}, (vec2){1.0f, 2.0f}, match));
    print_test(name, "to vec2 < 0.0,  0.0, 0.0, 0.0> -> < 0.0, 0.0>", test_vec4_demote_vec2((vec4){0.0f, 0.0f, 0.0f, 0.0f}, (vec2){0.0f, 0.0f}, match));
    print_test(name, "to vec2 <-6.0, -3.0, 2.0, -8.0> -> <-6.0, -3.0>\n", test_vec4_demote_vec2((vec4){-6.0f, -3.0f, 2.0f, -8.0f}, (vec2){-6.0f, -3.0f}, match));

    // Demote 3D
    print_test(name, "to vec3 < 1.0,  2.0, 3.0, 4.0> -> < 1.0, 2.0, 3.0>", test_vec4_demote_vec3((vec4){1.0f, 2.0f, 3.0f, 4.0f}, (vec3){1.0f, 2.0f, 3.0f}, match));
    print_test(name, "to vec3 < 0.0,  0.0, 0.0, 0.0> -> < 0.0, 0.0, 0.0>", test_vec4_demote_vec3((vec4){0.0f, 0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 0.f}, match));
    print_test(name, "to vec3 <-6.0, -3.0, 2.0, -8.0> -> <-6.0, -3.0, 2.0>\n", test_vec4_demote_vec3((vec4){-6.0f, -3.0f, 2.0f, -8.0f}, (vec3){-6.0f, -3.0f, 2.0f}, match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_linear_mat2 ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

    // Identity matrix
    print_test(name, "identity\n", test_mat2_identity((mat2){.a=1.0f,.b=0.0f,.c=0.0f,.d=1.0f}, match));

    // Matrix vector multiplication
    print_test(name, "mul vec2 [[ 0,  0], [ 0,  0]] * <1, 1>", test_mat2_mul_vec2((mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f}, (vec2){1.0f,1.0f}, (vec2){0.0f,0.0f}, match));
    
    print_test(name, "mul vec2 [[ 1,  0], [ 0,  0]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=1.f,.b=0.f,.c=0.f,.d=0.f}, (vec2){5.0f,10.0f}, (vec2){5.0f,0.0f}, match));
    print_test(name, "mul vec2 [[ 0,  1], [ 0,  0]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=1.f,.c=0.f,.d=0.f}, (vec2){5.0f,10.0f}, (vec2){10.0f,0.0f}, match));
    print_test(name, "mul vec2 [[ 0,  0], [ 1,  0]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=0.f,.c=1.f,.d=0.f}, (vec2){5.0f,10.0f}, (vec2){0.0f,5.0f}, match));
    print_test(name, "mul vec2 [[ 0,  0], [ 0,  1]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=0.f,.c=0.f,.d=1.f}, (vec2){5.0f,10.0f}, (vec2){0.0f,10.0f}, match));
    
    print_test(name, "mul vec2 [[-1,  0], [ 0,  0]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=-1.f,.b=0.f,.c=0.f,.d=0.f}, (vec2){5.0f,10.0f}, (vec2){-5.0f, 0.f}, match));
    print_test(name, "mul vec2 [[ 0, -1], [ 0,  0]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=-1.f,.c=0.f,.d=0.f}, (vec2){5.0f,10.0f}, (vec2){-10.0f, 0.f}, match));
    print_test(name, "mul vec2 [[ 0,  0], [-1,  0]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=0.f,.c=-1.f,.d=0.f}, (vec2){5.0f,10.0f}, (vec2){0.0f,-5.0f}, match));
    print_test(name, "mul vec2 [[ 0,  0], [ 0, -1]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=0.f,.c=0.f,.d=-1.f}, (vec2){5.0f,10.0f}, (vec2){.0f,-10.0f}, match));

    print_test(name, "mul vec2 [[ 1,  0], [ 0,  1]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f}, (vec2){5.0f,10.0f}, (vec2){5.0f,10.0f}, match));
    print_test(name, "mul vec2 [[ 0,  1], [ 1,  0]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f}, (vec2){5.0f,10.0f}, (vec2){10.0f,5.0f}, match));
    print_test(name, "mul vec2 [[-1,  0], [ 0  -1]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f}, (vec2){5.0f,10.0f}, (vec2){-5.0f,-10.0f}, match));
    print_test(name, "mul vec2 [[ 0, -1], [-1,  0]] * <5, 10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f}, (vec2){5.0f,10.0f}, (vec2){-10.0f,-5.0f}, match));

    print_test(name, "mul vec2 [[ 1,  0], [ 0,  1]] * <-5, -10>", test_mat2_mul_vec2((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f}, (vec2){-5.0f,-10.0f}, (vec2){-5.0f,-10.0f}, match));
    print_test(name, "mul vec2 [[ 0,  1], [ 1,  0]] * <-5, -10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f}, (vec2){-5.0f,-10.0f}, (vec2){-10.0f,-5.0f}, match));
    print_test(name, "mul vec2 [[-1,  0], [ 0, -1]] * <-5, -10>", test_mat2_mul_vec2((mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f}, (vec2){-5.0f,-10.0f}, (vec2){5.0f,10.0f}, match));
    print_test(name, "mul vec2 [[ 0, -1], [-1,  0]] * <-5, -10>", test_mat2_mul_vec2((mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f}, (vec2){-5.0f,-10.0f}, (vec2){10.0f,5.0f}, match));

    print_test(name, "mul vec2 [[ 1,  2], [ 3,  4]] * <5, 7>\n", test_mat2_mul_vec2((mat2){.a=1.f,.b=2.f,.c=3.f,.d=4.f}, (vec2){5.0f,7.0f}, (vec2){19.f, 43.f}, match));
 
    // Matrix matrix multiplication
    print_test(name, "mul mat2 [[ 0,  0], [ 0,  0]] * [[ 0,  0], [ 0,  0]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f}, match));
    print_test(name, "mul mat2 [[ 1,  2], [ 3,  4]] * [[ 0,  0], [ 0,  0]]", test_mat2_mul_mat2((mat2){.a=1.f,.b=2.f,.c=3.f,.d=4.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f}, match));
    print_test(name, "mul mat2 [[ 0,  0], [ 0,  0]] * [[-4, -3], [-2, -1]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},(mat2){.a=-4.f,.b=-3.f,.c=-2.f,.d=-1.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f}, match));
    
    print_test(name, "mul mat2 [[ 1,  0], [ 0,  1]] * [[ 1,  0], [ 0,  1]]", test_mat2_mul_mat2((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f}, match));
    print_test(name, "mul mat2 [[ 1,  0], [ 0,  1]] * [[ 0,  1], [ 1,  0]]", test_mat2_mul_mat2((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f}, match));
    print_test(name, "mul mat2 [[ 1,  0], [ 0,  1]] * [[-1,  0], [ 0, -1]]", test_mat2_mul_mat2((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f}, match));
    print_test(name, "mul mat2 [[ 1,  0], [ 0,  1]] * [[ 0, -1], [-1,  0]]", test_mat2_mul_mat2((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=-0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f}, match));

    print_test(name, "mul mat2 [[ 0,  1], [ 1,  0]] * [[ 1,  0], [ 0,  1]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f}, match));
    print_test(name, "mul mat2 [[ 0,  1], [ 1,  0]] * [[ 0,  1], [ 1,  0]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f}, match));
    print_test(name, "mul mat2 [[ 0,  1], [ 1,  0]] * [[-1,  0], [ 0, -1]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f}, match));
    print_test(name, "mul mat2 [[ 0,  1], [ 1,  0]] * [[ 0, -1], [-1,  0]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f}, match));

    print_test(name, "mul mat2 [[-1,  0], [ 0, -1]] * [[ 1,  0], [ 0,  1]]", test_mat2_mul_mat2((mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f}, match));
    print_test(name, "mul mat2 [[-1,  0], [ 0, -1]] * [[ 0,  1], [ 1,  0]]", test_mat2_mul_mat2((mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f}, match));
    print_test(name, "mul mat2 [[-1,  0], [ 0, -1]] * [[-1,  0], [ 0, -1]]", test_mat2_mul_mat2((mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f}, match));
    print_test(name, "mul mat2 [[-1,  0], [ 0, -1]] * [[ 0, -1], [-1,  0]]", test_mat2_mul_mat2((mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=-0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f}, match));

    print_test(name, "mul mat2 [[ 0, -1], [-1,  0]] * [[ 1,  0], [ 0,  1]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f}, match));
    print_test(name, "mul mat2 [[ 0, -1], [-1,  0]] * [[ 0,  1], [ 1,  0]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f}, match));
    print_test(name, "mul mat2 [[ 0, -1], [-1,  0]] * [[-1,  0], [ 0, -1]]", test_mat2_mul_mat2((mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f}, match));
    print_test(name, "mul mat2 [[ 0, -1], [-1,  0]] * [[ 0, -1], [-1,  0]]\n", test_mat2_mul_mat2((mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f}, match));

    // Transpose tests
    print_test(name, "transpose [[ 0,  0], [ 0,  0]]", test_mat2_transpose((mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},match));

    print_test(name, "transpose [[ 1,  0], [ 0,  0]]", test_mat2_transpose((mat2){.a=1.f,.b=0.f,.c=0.f,.d=0.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=0.f},match));
    print_test(name, "transpose [[ 0,  1], [ 0,  0]]", test_mat2_transpose((mat2){.a=0.f,.b=1.f,.c=0.f,.d=0.f},(mat2){.a=0.f,.b=0.f,.c=1.f,.d=0.f},match));
    print_test(name, "transpose [[ 0,  0], [ 1,  0]]", test_mat2_transpose((mat2){.a=0.f,.b=0.f,.c=1.f,.d=0.f},(mat2){.a=0.f,.b=1.f,.c=0.f,.d=0.f},match));
    print_test(name, "transpose [[ 0,  0], [ 0,  1]]", test_mat2_transpose((mat2){.a=0.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=1.f},match));

    print_test(name, "transpose [[ 1,  0], [ 0,  1]]", test_mat2_transpose((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},match));
    print_test(name, "transpose [[ 0,  1], [ 1,  0]]", test_mat2_transpose((mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},(mat2){.a=0.f,.b=1.f,.c=1.f,.d=0.f},match));
    print_test(name, "transpose [[-1,  0], [ 0, -1]]", test_mat2_transpose((mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},(mat2){.a=-1.f,.b=0.f,.c=0.f,.d=-1.f},match));
    print_test(name, "transpose [[ 0, -1], [-1,  0]]", test_mat2_transpose((mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f},(mat2){.a=0.f,.b=-1.f,.c=-1.f,.d=0.f},match));
    
    print_test(name, "transpose [[ 1,  1], [ 0,  0]]", test_mat2_transpose((mat2){.a=1.f,.b=1.f,.c=0.f,.d=0.f},(mat2){.a=1.f,.b=0.f,.c=1.f,.d=0.f},match));
    print_test(name, "transpose [[ 0,  0], [ 1,  1]]", test_mat2_transpose((mat2){.a=0.f,.b=0.f,.c=1.f,.d=1.f},(mat2){.a=0.f,.b=1.f,.c=0.f,.d=1.f},match));
    print_test(name, "transpose [[ 1,  0], [ 1,  0]]", test_mat2_transpose((mat2){.a=1.f,.b=0.f,.c=1.f,.d=0.f},(mat2){.a=1.f,.b=1.f,.c=0.f,.d=0.f},match));
    print_test(name, "transpose [[ 0,  1], [ 0,  1]]", test_mat2_transpose((mat2){.a=0.f,.b=1.f,.c=0.f,.d=1.f},(mat2){.a=0.f,.b=0.f,.c=1.f,.d=1.f},match));

    print_test(name, "transpose [[ 1,  2], [ 3,  4]]", test_mat2_transpose((mat2){.a=1.f,.b=2.f,.c=3.f,.d=4.f},(mat2){.a=1.f,.b=3.f,.c=2.f,.d=4.f},match));
    print_test(name, "transpose [[ 4,  1], [ 2,  3]]", test_mat2_transpose((mat2){.a=4.f,.b=1.f,.c=2.f,.d=3.f},(mat2){.a=4.f,.b=2.f,.c=1.f,.d=3.f},match));
    print_test(name, "transpose [[ 3,  4], [ 1,  2]]", test_mat2_transpose((mat2){.a=3.f,.b=4.f,.c=1.f,.d=2.f},(mat2){.a=3.f,.b=1.f,.c=4.f,.d=2.f},match));
    print_test(name, "transpose [[ 2,  3], [ 4,  1]]\n", test_mat2_transpose((mat2){.a=2.f,.b=3.f,.c=4.f,.d=1.f},(mat2){.a=2.f,.b=4.f,.c=3.f,.d=1.f},match));

    // Promote 3x3
    print_test(name, "to mat3 [[0, 0], [0, 0]] -> [[0, 0, 0], [0, 0, 0], [0, 0, 1]]", test_mat2_promote_mat3((mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},match));
    print_test(name, "to mat3 [[1, 0], [0, 1]] -> [[1, 0, 0], [0, 1, 0], [0, 0, 1]]", test_mat2_promote_mat3((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},match));
    print_test(name, "to mat3 [[1, 2], [3, 4]] -> [[1, 2, 0], [3, 4, 0], [0, 0, 1]]\n", test_mat2_promote_mat3((mat2){.a=1.f,.b=2.f,.c=3.f,.d=4.f},(mat3){.a=1.f,.b=2.f,.c=0.f,.d=3.f,.e=4.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},match));

    // Promote 4x4
    print_test(name, "to mat4 [[0, 0], [0, 0]] -> [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]", test_mat2_promote_mat4((mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},(mat4){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=1.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},match));
    print_test(name, "to mat4 [[1, 0], [0, 1]] -> [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]", test_mat2_promote_mat4((mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},(mat4){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=1.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},match));
    print_test(name, "to mat4 [[1, 2], [3, 4]] -> [[1, 2, 0, 0], [3, 4, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]", test_mat2_promote_mat4((mat2){.a=1.f,.b=2.f,.c=3.f,.d=4.f},(mat4){.a=1.f,.b=2.f,.c=0.f,.d=0.f,.e=3.f,.f=4.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=1.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_linear_mat3 ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

    // Identity matrix
    print_test(name, "identity\n", test_mat3_identity((mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f}, match));

    // Matrix vector multiplication
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0,  0], [ 0,  0,  0]] * <1, 1, 1>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){1.0f,1.0f,1.0f}, (vec3){0.0f,0.0f,0.0f}, match));
    
    print_test(name, "mul vec3 [[ 1,  0,  0], [ 0,  0,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){5.0f,0.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  1,  0], [ 0,  0,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=1.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){10.0f,0.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  1], [ 0,  0,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){15.0f,0.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 1,  0,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=1.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,5.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  1,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,10.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0,  1], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,15.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0,  0], [ 1,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,0.0f,5.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0,  0], [ 0,  1,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=1.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,0.0f,10.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0,  0], [ 0,  0,  1]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,0.0f,15.0f}, match));

    print_test(name, "mul vec3 [[-1,  0,  0], [ 0,  0,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=-1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){-5.0f,0.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0, -1,  0], [ 0,  0,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=-1.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){-10.0f,0.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0, -1], [ 0,  0,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=-1.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){-15.0f,0.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [-1,  0,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=-1.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,-5.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0, -1,  0], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=-1.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,-10.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0, -1], [ 0,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=-1.f,.g=0.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,-15.0f,0.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0,  0], [-1,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=-1.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,0.0f,-5.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0,  0], [ 0, -1,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=-1.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,0.0f,-10.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  0], [ 0,  0,  0], [ 0,  0, -1]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=-1.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){0.0f,0.0f,-15.0f}, match));

    print_test(name, "mul vec3 [[ 1,  0,  0], [ 0,  1,  0], [ 0,  0,  1]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){5.0f,10.0f,15.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  1], [ 0,  1,  0], [ 1,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=1.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){15.0f,10.0f,5.0f}, match));
    print_test(name, "mul vec3 [[-1,  0,  0], [ 0, -1,  0], [ 0,  0, -1]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=-1.f,.b=0.f,.c=0.f,.d=0.f,.e=-1.f,.f=0.f,.g=0.f,.h=0.f,.i=-1.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){-5.0f,-10.0f,-15.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0, -1], [ 0, -1,  0], [-1,  0,  0]] * <5, 10, 15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=-1.f,.d=0.f,.e=-1.f,.f=0.f,.g=-1.f,.h=0.f,.i=0.f}, (vec3){5.0f,10.0f,15.0f}, (vec3){-15.0f,-10.0f,-5.0f}, match));

    print_test(name, "mul vec3 [[ 1,  0,  0], [ 0,  1,  0], [ 0,  0,  1]] * <-5, -10, -15>", test_mat3_mul_vec3((mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f}, (vec3){-5.0f,-10.0f,-15.0f}, (vec3){-5.0f,-10.0f,-15.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0,  1], [ 0,  1,  0], [ 1,  0,  0]] * <-5, -10, -15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=1.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f}, (vec3){-5.0f,-10.0f,-15.0f}, (vec3){-15.0f,-10.0f,-5.0f}, match));
    print_test(name, "mul vec3 [[-1,  0,  0], [ 0, -1,  0], [ 0,  0, -1]] * <-5, -10, -15>", test_mat3_mul_vec3((mat3){.a=-1.f,.b=0.f,.c=0.f,.d=0.f,.e=-1.f,.f=0.f,.g=0.f,.h=0.f,.i=-1.f}, (vec3){-5.0f,-10.0f,-15.0f}, (vec3){5.0f,10.0f,15.0f}, match));
    print_test(name, "mul vec3 [[ 0,  0, -1], [ 0, -1,  0], [-1,  0,  0]] * <-5, -10, -15>", test_mat3_mul_vec3((mat3){.a=0.f,.b=0.f,.c=-1.f,.d=0.f,.e=-1.f,.f=0.f,.g=-1.f,.h=0.f,.i=0.f}, (vec3){-5.0f,-10.0f,-15.0f}, (vec3){15.0f,10.0f,5.0f}, match));
    
    print_test(name, "mul vec3 [[ 1,  2,  3], [ 4,  5,  6], [ 7,  8,  9]] * <5, 7, 9>\n", test_mat3_mul_vec3((mat3){.a=1.f,.b=2.f,.c=3.f,.d=4.f,.e=5.f,.f=6.f,.g=7.f,.h=8.f,.i=9.f}, (vec3){5.0f,7.0f,9.0f}, (vec3){46.f, 109.f, 172.f}, match));
    
    // Matrix matrix multiplication
    log_info("[TODO]: g10 linear mat3 mul mat3\n\n");

    // Transpose tests
    print_test(name, "transpose [[ 0,  0,  0], [ 0,  0,  0], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},match));

    print_test(name, "transpose [[ 1,  0,  0], [ 0,  0,  0], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  1,  0], [ 0,  0,  0], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=1.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=1.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  1], [ 0,  0,  0], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  0], [ 1,  0,  0], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=1.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=1.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  0], [ 0,  1,  0], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  0], [ 0,  0,  1], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=1.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  0], [ 0,  0,  0], [ 1,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  0], [ 0,  0,  0], [ 0,  1,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=1.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  0], [ 0,  0,  0], [ 0,  0,  1]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},match));

    print_test(name, "transpose [[ 1,  0,  0], [ 0,  1,  0], [ 0,  0,  1]]", test_mat3_transpose((mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},(mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},match));
    print_test(name, "transpose [[ 0,  0,  1], [ 0,  1,  0], [ 1,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=1.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=1.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[-1,  0,  0], [ 0, -1,  0], [ 0,  0, -1]]", test_mat3_transpose((mat3){.a=-1.f,.b=0.f,.c=0.f,.d=0.f,.e=-1.f,.f=0.f,.g=0.f,.h=0.f,.i=-1.f},(mat3){.a=-1.f,.b=0.f,.c=0.f,.d=0.f,.e=-1.f,.f=0.f,.g=0.f,.h=0.f,.i=-1.f},match));
    print_test(name, "transpose [[ 0,  0, -1], [ 0, -1,  0], [-1,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=-1.f,.d=0.f,.e=-1.f,.f=0.f,.g=-1.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=-1.f,.d=0.f,.e=-1.f,.f=0.f,.g=-1.f,.h=0.f,.i=0.f},match));
    
    print_test(name, "transpose [[ 1,  1,  1], [ 0,  0,  0], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=1.f,.b=1.f,.c=1.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=1.f,.b=0.f,.c=0.f,.d=1.f,.e=0.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  0], [ 1,  1,  1], [ 0,  0,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=1.f,.e=1.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f},(mat3){.a=0.f,.b=1.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=1.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  0], [ 0,  0,  0], [ 1,  1,  1]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=1.f,.h=1.f,.i=1.f},(mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=1.f},match));

    print_test(name, "transpose [[ 1,  0,  0], [ 1,  0,  0], [ 1,  0,  0]]", test_mat3_transpose((mat3){.a=1.f,.b=0.f,.c=0.f,.d=1.f,.e=0.f,.f=0.f,.g=1.f,.h=0.f,.i=0.f},(mat3){.a=1.f,.b=1.f,.c=1.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  1,  0], [ 0,  1,  0], [ 0,  1,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=1.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=1.f,.i=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=1.f,.e=1.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  0,  1], [ 0,  0,  1], [ 0,  0,  1]]", test_mat3_transpose((mat3){.a=0.f,.b=0.f,.c=1.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=1.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=1.f,.h=1.f,.i=1.f},match));

    print_test(name, "transpose [[ 0,  2,  2], [ 4,  0,  2], [ 4,  4,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=2.f,.c=2.f,.d=4.f,.e=0.f,.f=2.f,.g=4.f,.h=4.f,.i=0.f},(mat3){.a=0.f,.b=4.f,.c=4.f,.d=2.f,.e=0.f,.f=4.f,.g=2.f,.h=2.f,.i=0.f},match));
    print_test(name, "transpose [[ 0,  4,  4], [ 2,  0,  4], [ 2,  2,  0]]", test_mat3_transpose((mat3){.a=0.f,.b=4.f,.c=4.f,.d=2.f,.e=0.f,.f=4.f,.g=2.f,.h=2.f,.i=0.f},(mat3){.a=0.f,.b=2.f,.c=2.f,.d=4.f,.e=0.f,.f=2.f,.g=4.f,.h=4.f,.i=0.f},match));

    print_test(name, "transpose [[ 1,  2,  3], [ 4,  5,  6], [ 7,  8,  9]]", test_mat3_transpose((mat3){.a=1.f,.b=2.f,.c=3.f,.d=4.f,.e=5.f,.f=6.f,.g=7.f,.h=8.f,.i=9.f},(mat3){.a=1.f,.b=4.f,.c=7.f,.d=2.f,.e=5.f,.f=8.f,.g=3.f,.h=6.f,.i=9.f},match));
    print_test(name, "transpose [[ 2,  3,  4], [ 5,  6,  7], [ 8,  9,  1]]", test_mat3_transpose((mat3){.a=2.f,.b=3.f,.c=4.f,.d=5.f,.e=6.f,.f=7.f,.g=8.f,.h=9.f,.i=1.f},(mat3){.a=2.f,.b=5.f,.c=8.f,.d=3.f,.e=6.f,.f=9.f,.g=4.f,.h=7.f,.i=1.f},match));
    print_test(name, "transpose [[ 3,  4,  5], [ 6,  7,  8], [ 9,  1,  2]]", test_mat3_transpose((mat3){.a=3.f,.b=4.f,.c=5.f,.d=6.f,.e=7.f,.f=8.f,.g=9.f,.h=1.f,.i=2.f},(mat3){.a=3.f,.b=6.f,.c=9.f,.d=4.f,.e=7.f,.f=1.f,.g=5.f,.h=8.f,.i=2.f},match));
    print_test(name, "transpose [[ 4,  5,  6], [ 7,  8,  9], [ 1,  2,  3]]", test_mat3_transpose((mat3){.a=4.f,.b=5.f,.c=6.f,.d=7.f,.e=8.f,.f=9.f,.g=1.f,.h=2.f,.i=3.f},(mat3){.a=4.f,.b=7.f,.c=1.f,.d=5.f,.e=8.f,.f=2.f,.g=6.f,.h=9.f,.i=3.f},match));
    print_test(name, "transpose [[ 5,  6,  7], [ 8,  9,  1], [ 2,  3,  4]]", test_mat3_transpose((mat3){.a=5.f,.b=6.f,.c=7.f,.d=8.f,.e=9.f,.f=1.f,.g=2.f,.h=3.f,.i=4.f},(mat3){.a=5.f,.b=8.f,.c=2.f,.d=6.f,.e=9.f,.f=3.f,.g=7.f,.h=1.f,.i=4.f},match));
    print_test(name, "transpose [[ 6,  7,  8], [ 9,  1,  2], [ 3,  4,  5]]", test_mat3_transpose((mat3){.a=6.f,.b=7.f,.c=8.f,.d=9.f,.e=1.f,.f=2.f,.g=3.f,.h=4.f,.i=5.f},(mat3){.a=6.f,.b=9.f,.c=3.f,.d=7.f,.e=1.f,.f=4.f,.g=8.f,.h=2.f,.i=5.f},match));
    print_test(name, "transpose [[ 7,  8,  9], [ 1,  2,  3], [ 4,  5,  6]]", test_mat3_transpose((mat3){.a=7.f,.b=8.f,.c=9.f,.d=1.f,.e=2.f,.f=3.f,.g=4.f,.h=5.f,.i=6.f},(mat3){.a=7.f,.b=1.f,.c=4.f,.d=8.f,.e=2.f,.f=5.f,.g=9.f,.h=3.f,.i=6.f},match));
    print_test(name, "transpose [[ 8,  9,  1], [ 2,  3,  4], [ 5,  6,  7]]", test_mat3_transpose((mat3){.a=8.f,.b=9.f,.c=1.f,.d=2.f,.e=3.f,.f=4.f,.g=5.f,.h=6.f,.i=7.f},(mat3){.a=8.f,.b=2.f,.c=5.f,.d=9.f,.e=3.f,.f=6.f,.g=1.f,.h=4.f,.i=7.f},match));
    print_test(name, "transpose [[ 9,  1,  2], [ 3,  4,  5], [ 6,  7,  8]]\n", test_mat3_transpose((mat3){.a=9.f,.b=1.f,.c=2.f,.d=3.f,.e=4.f,.f=5.f,.g=6.f,.h=7.f,.i=8.f},(mat3){.a=9.f,.b=3.f,.c=6.f,.d=1.f,.e=4.f,.f=7.f,.g=2.f,.h=5.f,.i=8.f},match));

    // Demomote 2x2
    print_test(name, "to mat2 [[0, 0, 0], [0, 0, 0], [0, 0, 0]] -> [[0, 0], [0, 0]]", test_mat3_demote_mat2((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},match));
    print_test(name, "to mat2 [[1, 0, 0], [0, 1, 0], [0, 0, 1]] -> [[1, 0], [0, 1]]", test_mat3_demote_mat2((mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},match));
    print_test(name, "to mat2 [[1, 2, 3], [4, 5, 6], [7, 8, 9]] -> [[1, 2], [4, 5]]\n", test_mat3_demote_mat2((mat3){.a=1.f,.b=2.f,.c=3.f,.d=4.f,.e=5.f,.f=6.f,.g=7.f,.h=8.f,.i=9.f},(mat2){.a=1.f,.b=2.f,.c=4.f,.d=5.f},match));
    
    // Promote 4x4
    print_test(name, "to mat4 [[0, 0, 0], [0, 0, 0], [0, 0, 0]] -> [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 1]]", test_mat3_promote_mat4((mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},(mat4){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=0.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},match));
    print_test(name, "to mat4 [[1, 0, 0], [0, 1, 0], [0, 0, 1]] -> [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]", test_mat3_promote_mat4((mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},(mat4){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=1.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},match));
    print_test(name, "to mat4 [[1, 2, 3], [4, 5, 6], [7, 8, 9]] -> [[1, 2, 3, 0], [4, 5, 6, 0], [7, 8, 9, 0], [0, 0, 0, 1]]", test_mat3_promote_mat4((mat3){.a=1.f,.b=2.f,.c=3.f,.d=4.f,.e=5.f,.f=6.f,.g=7.f,.h=8.f,.i=9.f},(mat4){.a=1.f,.b=2.f,.c=3.f,.d=0.f,.e=4.f,.f=5.f,.g=6.f,.h=0.f,.i=7.f,.j=8.f,.k=9.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_linear_mat4 ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

    // Identity matrix
    print_test(name, "identity\n", test_mat4_identity((mat4){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=1.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f}, match));

    // Matrix vector multiplication
    log_info("[TODO]: g10 linear mat4 mul vec4\n\n");

    // Matrix matrix multiplication
    log_info("[TODO]: g10 linear mat4 mul mat4\n\n");

    // Transpose tests
    log_info("[TODO]: g10 linear mat4 transpose\n\n");

    // Demote 2x2
    print_test(name, "to mat2 [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]   , [0, 0, 0, 0]]     -> [[0, 0, 0], [0, 0, 0], [0, 0, 0]]", test_mat4_demote_mat2((mat4){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=1.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},(mat2){.a=0.f,.b=0.f,.c=0.f,.d=0.f},match));
    print_test(name, "to mat2 [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0]   , [0, 0, 0, 1]]     -> [[1, 0, 0], [0, 1, 0], [0, 0, 1]]", test_mat4_demote_mat2((mat4){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=1.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},(mat2){.a=1.f,.b=0.f,.c=0.f,.d=1.f},match));
    print_test(name, "to mat2 [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]] -> [[1, 2, 3], [5, 6, 7], [9, 10, 11]]\n", test_mat4_demote_mat2((mat4){.a=1.f,.b=2.f,.c=3.f,.d=4.f,.e=5.f,.f=6.f,.g=7.f,.h=8.f,.i=9.f,.j=10.f,.k=11.f,.l=12.f,.m=13.f,.n=14.f,.o=15.f,.p=16.f},(mat2){.a=1.f,.b=2.f,.c=5.f,.d=6.f},match));

    // Demomote 3x3
    print_test(name, "to mat3 [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]   , [0, 0, 0, 0]]     -> [[0, 0], [0, 0]]", test_mat4_demote_mat3((mat4){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=0.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=0.f},(mat3){.a=0.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=0.f,.g=0.f,.h=0.f,.i=0.f},match));
    print_test(name, "to mat3 [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0]   , [0, 0, 0, 1]]     -> [[1, 0], [0, 1]]", test_mat4_demote_mat3((mat4){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=0.f,.f=1.f,.g=0.f,.h=0.f,.i=0.f,.j=0.f,.k=1.f,.l=0.f,.m=0.f,.n=0.f,.o=0.f,.p=1.f},(mat3){.a=1.f,.b=0.f,.c=0.f,.d=0.f,.e=1.f,.f=0.f,.g=0.f,.h=0.f,.i=1.f},match));
    print_test(name, "to mat3 [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]] -> [[1, 2], [5, 6]]", test_mat4_demote_mat3((mat4){.a=1.f,.b=2.f,.c=3.f,.d=4.f,.e=5.f,.f=6.f,.g=7.f,.h=8.f,.i=9.f,.j=10.f,.k=11.f,.l=12.f,.m=13.f,.n=14.f,.o=15.f,.p=16.f},(mat3){.a=1.f,.b=2.f,.c=3.f,.d=5.f,.e=6.f,.f=7.f,.g=9.f,.h=10.f,.i=11.f},match));

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
              g10_vec3_t1 = 0,
              g10_vec4_t0 = 0,
              g10_vec4_t1 = 0;

    ///////////////
    // Test vec2 //
    ///////////////

    // Start timing core 
    g10_vec2_t0 = timer_high_precision();

        // Test vec2
        test_g10_linear_vec2("g10 linear vec2");

    // Stop timing core
    g10_vec2_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear vec2 took ");
    print_time_pretty ( (double)(g10_vec2_t1-g10_vec2_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");
    
    ///////////////
    // Test vec3 //
    ///////////////

    // Start timing core 
    g10_vec3_t0 = timer_high_precision();

        // Test vec3
        test_g10_linear_vec3("g10 linear vec3");

    // Stop timing core
    g10_vec3_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear vec3 took ");
    print_time_pretty ( (double)(g10_vec3_t1-g10_vec3_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");

    ///////////////
    // Test vec4 //
    ///////////////

    // Start timing core 
    g10_vec4_t0 = timer_high_precision();

        // Test vec4
        test_g10_linear_vec4("g10 linear vec4");

    // Stop timing core
    g10_vec4_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear vec4 took ");
    print_time_pretty ( (double)(g10_vec4_t1-g10_vec4_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");

    // Success
    return;
}

void test_g10_linear_matrices ( const char *name )
{

    // Initialized data
    timestamp g10_mat2_t0 = 0,
              g10_mat2_t1 = 0,
              g10_mat3_t0 = 0,
              g10_mat3_t1 = 0,
              g10_mat4_t0 = 0,
              g10_mat4_t1 = 0;

    ///////////////
    // Test mat2 //
    ///////////////

    // Start timing core 
    g10_mat2_t0 = timer_high_precision();

        // Test mat2
        test_g10_linear_mat2("g10 linear mat2");

    // Stop timing core
    g10_mat2_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear mat2 took ");
    print_time_pretty ( (double)(g10_mat2_t1-g10_mat2_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");
    
    ///////////////
    // Test mat3 //
    ///////////////

    // Start timing core 
    g10_mat3_t0 = timer_high_precision();

        // Test mat3
        test_g10_linear_mat3("g10 linear mat3");

    // Stop timing core
    g10_mat3_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear mat3 took ");
    print_time_pretty ( (double)(g10_mat3_t1-g10_mat3_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");

    ///////////////
    // Test mat4 //
    ///////////////

    // Start timing core 
    g10_mat4_t0 = timer_high_precision();

        // Test mat4
        test_g10_linear_mat4("g10 linear mat4");

    // Stop timing core
    g10_mat4_t1 = timer_high_precision();

    // Report the time it took to run the core tests
    log_info("g10 linear mat4 took ");
    print_time_pretty ( (double)(g10_mat4_t1-g10_mat4_t0)/(double)timer_seconds_divisor() );
    log_info(" to test\n\n");

    // Success
    return;
}

void test_g10_quaternion ( const char *name )
{

    // Formatting
    log_scenario("%s\n", name);

    // Identity quaternion
    print_test(name, "identity", test_quaternion_identity((quaternion){.u=1.f,.i=0.f,.j=0.f,.k=0.f}, match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_transform ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);
    
    // Transform
    print_test(name, "empty", test_transform_from_json("test cases/transform/empty.json", (void *) 0, match));
    print_test(name, "empty array", test_transform_from_json("test cases/transform/empty.json", (void *) 0, match));
    print_test(name, "empty object", test_transform_from_json("test cases/transform/empty.json", (void *) 0, match));

    // Location property
    print_test(name, "location too long", test_transform_from_json("test cases/transform/location_too_long.json", (void *) 0, match));
    print_test(name, "location too short", test_transform_from_json("test cases/transform/location_too_short.json", (void *) 0, match));
    print_test(name, "location wrong type", test_transform_from_json("test cases/transform/location_wrong_type.json", (void *) 0, match));

    // Rotation property
    print_test(name, "rotation too long", test_transform_from_json("test cases/transform/location_too_long.json", (void *) 0, match));
    print_test(name, "rotation too short", test_transform_from_json("test cases/transform/location_too_short.json", (void *) 0, match));
    print_test(name, "rotation wrong type", test_transform_from_json("test cases/transform/location_wrong_type.json", (void *) 0, match));

    // Quaternion property
    print_test(name, "quaternion too long", test_transform_from_json("test cases/transform/quaternion_too_long.json", (void *) 0, match));
    print_test(name, "quaternion too short", test_transform_from_json("test cases/transform/quaternion_too_short.json", (void *) 0, match));
    print_test(name, "quaternion wrong type", test_transform_from_json("test cases/transform/quaternion_wrong_type.json", (void *) 0, match));
    
    // Scale property
    print_test(name, "scale too long", test_transform_from_json("test cases/transform/scale_too_long.json", (void *) 0, match));
    print_test(name, "scale too short", test_transform_from_json("test cases/transform/scale_too_short.json", (void *) 0, match));
    print_test(name, "scale wrong type", test_transform_from_json("test cases/transform/scale_wrong_type.json", (void *) 0, match));

    // Missing properties
    print_test(name, "missing location", test_transform_from_json("test cases/transform/missing_location.json", (void *) 0, match));
    print_test(name, "missing rotation", test_transform_from_json("test cases/transform/missing_rotation.json", (void *) 0, match));
    print_test(name, "missing scale", test_transform_from_json("test cases/transform/missing_scale.json", (void *) 0, match));
    print_test(name, "missing location rotation", test_transform_from_json("test cases/transform/missing_location_rotation.json", (void *) 0, match));
    print_test(name, "missing location scale", test_transform_from_json("test cases/transform/missing_location_scale.json", (void *) 0, match));
    print_test(name, "missing rotation scale", test_transform_from_json("test cases/transform/missing_rotation_scale.json", (void *) 0, match));
    print_test(name, "missing location rotation scale", test_transform_from_json("test cases/transform/missing_location_rotation_scale.json", (void *) 0, match));

    // Valid transform
    print_test(name, "valid", test_transform_from_json("test cases/transform/valid.json", (void *) 0, match));

    // Print the summary of this test
    print_final_summary();

    // Success
    return;
}

void test_g10_user_code ( const char *name )
{
    
    // Formatting
    log_scenario("%s\n", name);

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
