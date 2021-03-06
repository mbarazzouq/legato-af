/** @page c_hashmap HashMap API
 *
 *
 * @ref le_hashmap.h "API Reference"
 *
 * <HR>
 *
 * @ref c_hashmap_create <br>
 * @ref c_hashmap_insert <br>
 * @ref c_hashmap_iterating <br>
 * @ref c_hashmap_tracing <br>
 *
 *
 * This API provides a straightforward HashMap implementation.
 *
 * @section c_hashmap_create Creating a HashMap
 *
 * Use @c le_hashmap_Create() to create a hashmap. It's the responsibility of
 * the caller to maintain type integrity using this function's parameters.
 * It's important to supply hash and equality functions that operate on the
 * type of key that you intend to store. It's unwise to mix types in a single table because
 * implementation of the table has no way to detect this behaviour.
 *
 * Choose the initial size should carefully as the index size remains fixed. The best
 * choice for the initial size is a prime number slightly larger than the
 * maximum expected capacity. If a too small size is chosen, there will be an
 * increase in collisions that degrade performance over time.
 *
 * All hashmaps have names for diagnostic purposes.
 *
 * @section c_hashmap_insert Adding key-value pairs
 *
 * Key-value pairs are added using le_hashmap_Put(). For example:
 *
 * @code
 * static void StoreStuff(const char* keyStr, const char* valueStr)
 * {
 *     myTable = le_hashmap_Create(
 *                         "My Table",
 *                         31,
 *                         le_hashmap_HashString,
 *                         le_hashmap_EqualsString
 *                       );
 *
 *     le_hashmap_Put(myTable, keyStr, valueStr);
 *     ....
 * }
 * @endcode
 *
 * The table does not take control of the keys or values. The map only stores the pointers
 * to these values, not the values themselves. It's the responsibility of the caller to manage
 * the actual data storage.
 *
 * @subsection c_hashmap_tips Tip
 *
 * The code sample shows some pre-defined functions for certain
 * key types. The key types supported are uint32_t and strings. The strings must be
 * NULL terminated.
 *
 * Tables can also have their own hash and equality functions,
 * but ensure the functions work on the type of key you're
 * storing. The hash function should provide a good distribution of values. It
 * is not required that they be unique.
 *
 * @section c_hashmap_iterating Iterating over a map
 *
 * This API allows the user of the map to iterate over the entire
 * map, acting on each key-value pair. You supply a callback function conforming
 * to the prototype:
 * @code
 * bool (*callback)(void* key, void* value, void* context)
 * @endcode
 *
 * This can then be used to process every value in the map. The return value from the
 * callback function determines if iteration should continue or stop. If the function
 * returns false then iteration will cease. For example:
 *
 * @code
 * bool ProcessTableData
 * (
 *     void* keyPtr,     // Pointer to the map entry's key
 *     void* valuePtr,   // Pointer to the map entry's value
 *     void* contextPtr  // Pointer to an abritrary context for the callback function
 * )
 * {
 *     int keyCheck = *((int*)context);
 *     int currentKey = *((int*)key);
 *
 *     if (keyCheck == currentKey) return false;
 *
 *     // Do some stuff, maybe print out data or do a calculation
 *     return true;
 * }
 *
 * {
 *     // ... somewhere else in the code ...
 *     int lastKey = 10;
 *     le_hashmap_ForEach (myTable, processTableData, &lastKey);
 * }
 * @endcode
 *
 * This code sample shows the callback function must also be aware of the
 * types stored in the table.
 *
 * However, keep in mind that it is unsafe and undefined to modify the map during
 * this style of iteration.
 *
 * Alternatively, the calling function can control the iteration by first
 * calling @c le_hashmap_GetIterator(). This returns an iterator that is ready
 * to return each key/value pair in the map in the order in which they are
 * stored. The iterator is controlled by calling @c le_hashmap_NextNode(), and must
 * be called before accessing any elements. You can then retrieve pointers to
 * the key and value by using le_hashmap_GetKey() and le_hashmap_GetValue().
 *
 * @note There is only one iterator per hashtable. Calling le_hashmap_GetIterator()
 * will simply re-initialize the current iterator
 *
 * It is possible to add and remove items during this style of iteration.  When
 * adding items during an iteration it is not guaranteed that the newly added item
 * will be iterated over.  It's very possible that the newly added item is added in
 * an earlier location than the iterator is curently pointed at.
 *
 * When removing items during an iteration you also have to keep in mind that the
 * iterator's current item may be the one removed.  If this is the case,
 * le_hashmap_GetKey, and le_hashmap_GetValue will return NULL until either,
 * le_hashmap_NextNode, or le_hashmap_PrevNode are called.
 *
 * For example (assuming a table of string/string):
 *
 * @code
 * void ProcessTable(le_hashmap_Ref_t myTable)
 * {
 *     char* nextKey;
 *     char* nextVal;
 *
 *     le_hashmap_It_Ref_t myIter = le_hashmap_GetIterator(myTable);
 *
 *     while (LE_OK == le_hashmap_NextNode(myIter))
 *     {
 *         // Do something with the strings
 *         nextKey = le_hashmap_GetKey(myIter);
 *         nextVal = le_hashmap_GetValue(myIter);
 *     }
 * }
 * @endcode
 *
 * If you need to control access to the hashmap, then a mutex can be used.
 *
 * @section c_hashmap_tracing Tracing a map
 *
 * Hashmaps can be traced using the logging system.
 *
 * If @c le_hashmap_MakeTraceable() is called for a specified hashmap object, the name of that
 * hashmap (the name passed into le_hashmap_Create() ) becomes a trace keyword
 * to enable and disable tracing of that particular hashmap.
 *
 * If @c le_hashmap_EnableTrace() is called for a hashmap object, tracing is
 * immediately activated for that hashmap.
 *
 * See @ref c_log_controlling for more information on how to enable and disable tracing using
 * configuration and/or the log control tool.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */

/** @file le_hashmap.h
 *
 * Legato @ref c_hashmap include file.
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */

#ifndef LEGATO_HASHMAP_INCLUDE_GUARD
#define LEGATO_HASHMAP_INCLUDE_GUARD

//--------------------------------------------------------------------------------------------------
/**
 * Reference to a HashMap.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_hashmap* le_hashmap_Ref_t;

//--------------------------------------------------------------------------------------------------
/**
 * Reference to a HashMap Iterator.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_hashmap_It* le_hashmap_It_Ref_t;

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for hash functions.  The hash function must generate a good spread of hashes without
 * consuming lots of processing power.
 *
 * @param keyToHashPtr Pointer to the key which will be hashed
 * @return The calculated hash value
 */
//--------------------------------------------------------------------------------------------------
typedef size_t (*le_hashmap_HashFunc_t)
(
    const void* keyToHashPtr
);

//--------------------------------------------------------------------------------------------------
/**
 * Prototype for equality functions.  The equality function returns true if the the keys point to
 * values are equivalent. The HashMap doesn't know in advance which types are to be stored so
 * this function must be supplied by the caller.
 *
 * @param firstKeyPtr Pointer to the first key for comparing.
 * @param secondKeyPtr Pointer to the second key for comparing.
 * @return Returns true if the values are the same, false otherwise
 */
//--------------------------------------------------------------------------------------------------
typedef bool (*le_hashmap_EqualsFunc_t)
(
    const void* firstKeyPtr,
    const void* secondKeyPtr
);


//--------------------------------------------------------------------------------------------------
/**
 * Prototype for callback functions for the iterator function le_hashmap_ForEach().  This function should
 * return true in order to continue iterating, or false to stop.
 *
 * @param keyPtr Pointer to the key at the current position in the map
 * @param valuePtr Pointer to the value associated to this key
 * @param contextPtr Pointer to the context supplied to le_hashmap_ForEach()
 * @return Returns true to continue, false to stop
 */
//--------------------------------------------------------------------------------------------------
typedef bool (*le_hashmap_ForEachHandler_t)
(
    const void* keyPtr,
    const void* valuePtr,
    void* contextPtr
);

//--------------------------------------------------------------------------------------------------
/**
 * Create a HashMap.
 *
 * If you create a hashmap with a smaller capacity than you actually use, then
 * the map will continue to work, but performance will degrade the more you put in the map.
 *
 * @return  Returns a reference to the map.
 *
 * @note Terminates the process on failure, so no need to check the return value for errors.
 */
//--------------------------------------------------------------------------------------------------
le_hashmap_Ref_t le_hashmap_Create
(
    const char*                nameStr,          ///< [in] Name of the HashMap
    size_t                     capacity,         ///< [in] Size of the hashmap
    le_hashmap_HashFunc_t      hashFunc,         ///< [in] Hash function
    le_hashmap_EqualsFunc_t    equalsFunc        ///< [in] Equality function
);

//--------------------------------------------------------------------------------------------------
/**
 * Add a key-value pair to a HashMap. If the key already exists in the map, the previous value
 * will be replaced with the new value passed into this function.
 *
 * @return  Returns NULL for a new entry or a pointer to the old value if it is replaced.
 *
 */
//--------------------------------------------------------------------------------------------------

void* le_hashmap_Put
(
    le_hashmap_Ref_t mapRef,   ///< [in] Reference to the map.
    const void* keyPtr,        ///< [in] Pointer to the key to be stored.
    const void* valuePtr       ///< [in] Pointer to the value to be stored.
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieve a value from a HashMap.
 *
 * @return  Returns a pointer to the value or NULL if the key is not found.
 *
 */
//--------------------------------------------------------------------------------------------------

void* le_hashmap_Get
(
    le_hashmap_Ref_t mapRef,   ///< [in] Reference to the map.
    const void* keyPtr         ///< [in] Pointer to the key to be retrieved.
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove a value from a HashMap.
 *
 * @return  Returns a pointer to the value or NULL if the key is not found.
 *
 */
//--------------------------------------------------------------------------------------------------

void* le_hashmap_Remove
(
   le_hashmap_Ref_t mapRef,       ///< [in] Reference to the map.
   const void* keyPtr             ///< [in] Pointer to the key to be removed.
);

//--------------------------------------------------------------------------------------------------
/**
 * Tests if the HashMap is empty (i.e. contains zero keys).
 *
 * @return  Returns true if empty, false otherwise.
 *
 */
//--------------------------------------------------------------------------------------------------

bool le_hashmap_isEmpty
(
    le_hashmap_Ref_t mapRef    ///< [in] Reference to the map.
);

//--------------------------------------------------------------------------------------------------
/**
 * Calculates the number of keys in the HashMap.
 *
 * @return  The number of keys in the HashMap.
 *
 */
//--------------------------------------------------------------------------------------------------

size_t le_hashmap_Size
(
    le_hashmap_Ref_t mapRef    ///< [in] Reference to the map.
);

//--------------------------------------------------------------------------------------------------
/**
 * Tests if the HashMap contains a particular key.
 *
 * @return  Returns true if the key is found, false otherwise.
 *
 */
//--------------------------------------------------------------------------------------------------

bool le_hashmap_ContainsKey
(
    le_hashmap_Ref_t mapRef,  ///< [in] Reference to the map.
    const void* keyPtr        ///< [in] Pointer to the key to be searched.
);

//--------------------------------------------------------------------------------------------------
/**
 * Deletes all the entries held in the hashmap. This will not delete the data pointed to by the
 * key and value pointers. That cleanup is the responsibility of the caller.
 * This allows the map to be re-used. Currently maps can't be deleted.
 *
 */
//--------------------------------------------------------------------------------------------------

void le_hashmap_RemoveAll
(
    le_hashmap_Ref_t mapRef    ///< [in] Reference to the map.
);

//--------------------------------------------------------------------------------------------------
/**
 * Iterates over the whole map, calling the supplied callback with each key-value pair. If the callback
 * returns false for any key then this function will return.
 *
 * @return  Returns nothing
 *
 */
//--------------------------------------------------------------------------------------------------
void le_hashmap_ForEach
(
    le_hashmap_Ref_t mapRef,                 ///< [in] Reference to the map.
    le_hashmap_ForEachHandler_t forEachFn,   ///< [in] Callback function to be called with each pair.
    void* contextPtr                         ///< [in] Pointer to a context to be supplied to the callback.
);

//--------------------------------------------------------------------------------------------------
/**
 * Gets an iterator for step-by-step iteration over the map. In this mode,
 * the iteration is controlled by the calling function using the le_hashmap_NextNode() function.
 * There is one iterator per map, and calling this function resets the
 * iterator position to the start of the map.
 * The iterator is not ready for data access until le_hashmap_NextNode() has been called
 * at least once.
 *
 * @return  Returns A reference to a hashmap iterator which is ready
 *          for le_hashmap_NextNode() to be called on it
 *
 */
//--------------------------------------------------------------------------------------------------
le_hashmap_It_Ref_t le_hashmap_GetIterator
(
    le_hashmap_Ref_t mapRef                 ///< [in] Reference to the map.
);

//--------------------------------------------------------------------------------------------------
/**
 * Moves the iterator to the next key/value pair in the map. Order is dependent
 * on the hash algorithm and the order of inserts, and is not sorted at all.
 *
 * @return  Returns LE_OK unless you go past the end of the map, then returns LE_NOT_FOUND.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_hashmap_NextNode
(
    le_hashmap_It_Ref_t iteratorRef        ///< [IN] Reference to the iterator.
);

//--------------------------------------------------------------------------------------------------
/**
 * Moves the iterator to the previous key/value pair in the map. Order is dependent
 * on the hash algorithm and the order of inserts, and is not sorted at all.
 *
 * @return  Returns LE_OK unless you go past the beginning of the map, then returns LE_NOT_FOUND.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_hashmap_PrevNode
(
    le_hashmap_It_Ref_t iteratorRef        ///< [IN] Reference to the iterator
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieves a pointer to the key where the iterator is currently pointing.
 * If the iterator has just been initialized and le_hashmap_NextNode() has not been
 * called, or if the iterator has been invalidated, this will return NULL.
 *
 * @return  Pointer to the current key, or NULL if the iterator has been invalidated or is not
 *          ready.
 *
 */
//--------------------------------------------------------------------------------------------------
void const * le_hashmap_GetKey
(
    le_hashmap_It_Ref_t iteratorRef        ///< [IN] Reference to the iterator.
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieves a pointer to the value where the iterator is currently pointing.
 * If the iterator has just been initialized and le_hashmap_NextNode() has not been
 * called, or if the iterator has been invalidated, this will return NULL.
 *
 * @return  Pointer to the current value, or NULL if the iterator has been invalidated or is not
 *          ready.
 *
 */
//--------------------------------------------------------------------------------------------------
void const * le_hashmap_GetValue
(
    le_hashmap_It_Ref_t iteratorRef        ///< [IN] Reference to the iterator.
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieves the key and value of the first node stored in the hashmap.
 * The hashmap is not sorted so this will simply return the first node stored in the map.
 * There is no guarantee that a subsequent call to this function will return the same pair
 * if new keys have been added to the map.
 * If NULL is passed as the firstValuePointer then only the key will be returned.
 *
 * @return  LE_OK if the first node is returned or LE_NOT_FOUND if the map is empty.
 *          LE_BAD_PARAMETER if the key is NULL.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_hashmap_GetFirstNode
(
    le_hashmap_Ref_t mapRef,   ///< [in] Reference to the map
    void **firstKeyPtr,        ///< [out] Pointer to the first key
    void **firstValuePtr       ///< [out] Pointer to the first value
);

//--------------------------------------------------------------------------------------------------
/**
 * Retrieves the key and value of the node after the passed in key.
 * The hashmap is not sorted so this will simply return the next node stored in the map.
 * There is no guarantee that a subsequent call to this function will return the same pair
 * if new keys have been added to the map.
 * If NULL is passed as the nextValuePtr then only the key will be returned.
 *
 * @return  LE_OK if the next node is returned. If the keyPtr is not found in the
 *          map then LE_BAD_PARAMETER is returned. LE_NOT_FOUND is returned if the passed
 *          in key is the last one in the map.
 *
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_hashmap_GetNodeAfter
(
    le_hashmap_Ref_t mapRef,   ///< [in] Reference to the map
    const void* keyPtr,        ///< [in] Pointer to the key to be searched for
    void **nextKeyPtr,         ///< [out] Pointer to the first key
    void **nextValuePtr        ///< [out] Pointer to the first value
);


//--------------------------------------------------------------------------------------------------
/**
 * Counts the total number of collisions in the map. A collision occurs
 * when more than one entry is stored in the map at the same index.
 *
 * @return  Returns the total collisions in the map.
 *
 */
//--------------------------------------------------------------------------------------------------

size_t le_hashmap_CountCollisions
(
    le_hashmap_Ref_t mapRef     ///< [in] Reference to the map.
);

//--------------------------------------------------------------------------------------------------
/**
 * String hashing function. Can be used as a parameter to le_hashmap_Create() if the key to
 * the table is a string.
 *
 * @return  Returns the hash value of the string pointed to by stringToHash.
 *
 */
//--------------------------------------------------------------------------------------------------

size_t le_hashmap_HashString
(
    const void* stringToHashPtr    ///< [in] Pointer to the string to be hashed.
);

//--------------------------------------------------------------------------------------------------
/**
 * String equality function. Can be used as a parameter to le_hashmap_Create() if the key to
 * the table is a string
 *
 * @return  Returns true if the strings are identical, false otherwise.
 *
 */
//--------------------------------------------------------------------------------------------------

bool le_hashmap_EqualsString
(
    const void* firstStringPtr,    ///< [in] Pointer to the first string for comparing.
    const void* secondStringPtr    ///< [in] Pointer to the second string for comparing.
);

//--------------------------------------------------------------------------------------------------
/**
 * Integer hashing function. Can be used as a parameter to le_hashmap_Create() if the key to
 * the table is a uint32_t.
 *
 * @return  Returns the hash value of the uint32_t pointed to by intToHash.
 *
 */
//--------------------------------------------------------------------------------------------------

size_t le_hashmap_HashUInt32
(
    const void* intToHashPtr    ///< [in] Pointer to the integer to be hashed.
);

//--------------------------------------------------------------------------------------------------
/**
 * Integer equality function. Can be used as a parameter to le_hashmap_Create() if the key to
 * the table is a uint32_t.
 *
 * @return  Returns true if the integers are equal, false otherwise.
 *
 */
//--------------------------------------------------------------------------------------------------

bool le_hashmap_EqualsUInt32
(
    const void* firstIntPtr,    ///< [in] Pointer to the first integer for comparing.
    const void* secondIntPtr    ///< [in] Pointer to the second integer for comparing.
);

//--------------------------------------------------------------------------------------------------
/**
 * Pointer hashing function. Can be used as a parameter to le_hashmap_Create() if the key to
 * the table is an pointer or reference. Simply pass in the address as the key.
 *
 * @return  Returns the hash value of the pointer pointed to by voidToHashPtr
 *
 */
//--------------------------------------------------------------------------------------------------

size_t le_hashmap_HashVoidPointer
(
    const void* voidToHashPtr    ///< [in] Pointer to be hashed
);

//--------------------------------------------------------------------------------------------------
/**
 * Pointer equality function. Can be used as a parameter to le_hashmap_Create() if the key to
 * the table is an pointer or reference.
 *
 * @return  Returns true if the pointers are equal, false otherwise
 *
 */
//--------------------------------------------------------------------------------------------------

bool le_hashmap_EqualsVoidPointer
(
    const void* firstVoidPtr,    ///< [in] First pointer for comparing.
    const void* secondVoidPtr    ///< [in] Second pointer for comparing.
);



//--------------------------------------------------------------------------------------------------
/**
 * Makes a particular hashmap traceable without enabling the tracing.  After this is called, when
 * the trace keyword for this hashmap (the hashmap's name) is enabled for the "framework" component
 * in the process, tracing will start.  If that keyword was enabled before
 * this function was called, tracing will start immediately when it is called.
 **/
//--------------------------------------------------------------------------------------------------
void le_hashmap_MakeTraceable
(
    le_hashmap_Ref_t mapRef     ///< [in] Reference to the map.
);


//--------------------------------------------------------------------------------------------------
/**
 * Immediately enables tracing on a particular hashmap object.
 **/
//--------------------------------------------------------------------------------------------------
void le_hashmap_EnableTrace
(
    le_hashmap_Ref_t mapRef     ///< [in] Reference to the map.
);


#endif /* LEGATO_HASHMAP_INCLUDE_GUARD */
