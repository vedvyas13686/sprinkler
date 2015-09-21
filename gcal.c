// -------------------------------------------------------------------------
// This software is furnished "as is", without technical
// support, and with no warranty, express or implied, as to its usefulness for
// any purpose.
//
// gcal.c 
//    This file acts as the wrapper around the sp_func.py python library
//
// Author: Vedvyas Shanbhogue
// ------------------------------------------------------------------------*/

#ifdef IDENT_C
static const char* const <filename>_c_Id = "$Id$";
#endif

#include <Python.h>

// -- global -----------------------------------------------------------------
// get_calendar_events()
//
// Returns:
//     0 - success 
//     1 - error
// On success return, the duration is set to the duration of the event
// if no event was found then duration is returned as 0
// -------------------------------------------------------------------------*/
int 
get_calendar_events(
    time_t *duration)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue;
    struct tm start_time, end_time;
    char *start, *end, *name;
    char *argv[] = {
        "sp",
        "sp_func",
        "get_cal_events"
    };

    // Initialize the Python Interpreter
    Py_Initialize();

    // Build the name object
    pName = PyString_FromString(argv[1]);

    // Load the module object
    PySys_SetArgv(1, argv);
    PyErr_Print();
    pModule = PyImport_Import(pName);
    PyErr_Print();

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference 
    pFunc = PyDict_GetItemString(pDict, argv[2]);

    if ( !PyCallable_Check(pFunc) ) {
        PyErr_Print();
        Py_DECREF(pModule);
        Py_DECREF(pName);
        Py_Finalize();
        return 1;
    }

    // Get the calendar events from google calendar
    pValue = PyObject_CallObject(pFunc, NULL);

    // Extract the start time, end time and event name strings
    if ( PyArg_ParseTuple(pValue, "sss", &start, &end, &name) == 0 )
    {
        PyErr_Print();
        Py_DECREF(pModule);
        Py_DECREF(pName);
        Py_Finalize();
        return 1;
    }

    *duration = 0;

    // check if any event returned
    if ( strlen(start) && strlen(end) && strlen(name) &&
         (strcmp(name, "Sprinkler On") == 0) ) {

        // Turn time strings into struct tm
        strptime(start, "%Y-%m-%dT%T", &start_time);
        strptime(end, "%Y-%m-%dT%T", &end_time);

        // Check if it is time to start the sprinker
        if ( time(NULL) >= mktime(&start_time) &&
             time(NULL) < mktime(&end_time) ) {
            // Time to start the sprinker for specified duration
            *duration = mktime(&end_time) - time(NULL);
        }
    }

    Py_DECREF(pModule);
    Py_DECREF(pName);
    Py_Finalize();
    return 0;
}
