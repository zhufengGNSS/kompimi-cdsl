/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

/*! \brief Define to perform bounds checking on each access by default */
#define DYNAMIC_ARRAY_BOUNDS_CHECKING 1

/* The implementation guarantees that the load on the dynamic array
   (size/capacity) is never below the minimum load. The loads after
   expansion/shrinking affect how often reallocations occur. Smaller
   after-expansion and larger after-shrinking loads lead to less
   expansions, at the cost of more space. */
#define DYNAMIC_ARRAY_MINIMUM_LOAD 0.4
#define DYNAMIC_ARRAY_LOAD_AFTER_EXPANSION 0.6
#define DYNAMIC_ARRAY_LOAD_AFTER_SHRINKING 0.8

/*! \brief Number of elements stored in each list node.
    Should be at most a small multiple of the cache line size.
    By default we set it to 72/sizeof(void*), since a Pentium 4
    cache line is 128 bytes, and the node metadata (12 bytes)
    plus the allocation metadata roughly fills out the rest.

    May not be less than 2.
*/
#define ELEMENTS_PER_LIST_NODE  ((int)((72)/sizeof(void*)))
