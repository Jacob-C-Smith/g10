#include <aabb.h>

int aabb_from_bounds ( aabb *p_aabb, vec3 min, vec3 max )
{

    // error check
    if ( NULL == p_aabb ) goto no_aabb;

    // populate the struct
    *p_aabb = (aabb)
    {
        ._min = min,
        ._max = max
    };

    // done
    return 1;
    
    no_aabb: return 0;
}

int aabb_from_transform ( aabb *p_aabb, transform *p_transform )
{

    // error check
    if ( NULL == p_aabb ) goto no_aabb;

    // TODO: 
    //

    // done
    return 1;
    
    no_aabb: return 0;
}

// function definitions
int aabb_info ( aabb *p_aabb )
{

    printf("AABB @%p\n", p_aabb);
    printf(" - min : < %.2f, %.2f, %.2f >\n", p_aabb->_min.x, p_aabb->_min.y, p_aabb->_min.z);
    printf(" - max : < %.2f, %.2f, %.2f >\n", p_aabb->_max.x, p_aabb->_max.y, p_aabb->_max.z);
    
    return 1;
}

const char *aabb_key_accessor ( const aabb *const p_aabb )
{

    // done
    return 0;
}

int aabb_equality ( const aabb *p_a, const aabb *p_b )
{

    return p_a == p_b;
}
