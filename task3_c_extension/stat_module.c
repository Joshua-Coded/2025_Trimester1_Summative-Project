/*
 * stat_module.c - C extension for Python providing statistical functions
 * 
 * Functions included:
 * - sum_array: Sum of all values in the array
 * - avg_array: Average value of the array
 * - std_dev_array: Standard deviation of values in the array
 * - mode_array: Mode value of the array
 * - count_array: Total count of values in the array
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>

// Function to calculate the sum of array values
static PyObject *
stat_module_sum_array(PyObject *self, PyObject *args)
{
    PyObject *list;
    double sum = 0.0;
    Py_ssize_t i, size;
    
    // Parse arguments - expect a list
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &list))
        return NULL;
    
    // Get the size of the list
    size = PyList_Size(list);
    
    // Calculate sum of values
    for (i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(list, i);
        
        // Check if item is numeric
        if (!PyFloat_Check(item) && !PyLong_Check(item)) {
            PyErr_SetString(PyExc_TypeError, "List must contain only numbers");
            return NULL;
        }
        
        sum += PyFloat_AsDouble(item);
    }
    
    return PyFloat_FromDouble(sum);
}

// Function to calculate the average value of the array
static PyObject *
stat_module_avg_array(PyObject *self, PyObject *args)
{
    PyObject *list;
    double sum = 0.0;
    Py_ssize_t i, size;
    
    // Parse arguments - expect a list
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &list))
        return NULL;
    
    // Get the size of the list
    size = PyList_Size(list);
    
    // Check if list is empty
    if (size == 0) {
        PyErr_SetString(PyExc_ValueError, "Cannot calculate average of empty list");
        return NULL;
    }
    
    // Calculate sum of values
    for (i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(list, i);
        
        // Check if item is numeric
        if (!PyFloat_Check(item) && !PyLong_Check(item)) {
            PyErr_SetString(PyExc_TypeError, "List must contain only numbers");
            return NULL;
        }
        
        sum += PyFloat_AsDouble(item);
    }
    
    return PyFloat_FromDouble(sum / size);
}

// Function to calculate the standard deviation of array values
static PyObject *
stat_module_std_dev_array(PyObject *self, PyObject *args)
{
    PyObject *list;
    double sum = 0.0, mean, std_dev = 0.0;
    Py_ssize_t i, size;
    
    // Parse arguments - expect a list
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &list))
        return NULL;
    
    // Get the size of the list
    size = PyList_Size(list);
    
    // Check if list is empty or has only one element
    if (size <= 1) {
        PyErr_SetString(PyExc_ValueError, "Need at least two values for standard deviation");
        return NULL;
    }
    
    // First pass: calculate the mean
    for (i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(list, i);
        
        // Check if item is numeric
        if (!PyFloat_Check(item) && !PyLong_Check(item)) {
            PyErr_SetString(PyExc_TypeError, "List must contain only numbers");
            return NULL;
        }
        
        sum += PyFloat_AsDouble(item);
    }
    mean = sum / size;
    
    // Second pass: calculate the standard deviation
    for (i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(list, i);
        double val = PyFloat_AsDouble(item);
        double diff = val - mean;
        std_dev += diff * diff;
    }
    
    std_dev = sqrt(std_dev / size);
    
    return PyFloat_FromDouble(std_dev);
}

// Function to find the mode of the array (most frequent value)
static PyObject *
stat_module_mode_array(PyObject *self, PyObject *args)
{
    PyObject *list;
    PyObject *count_dict = NULL;
    PyObject *mode = NULL;
    int max_count = 0;
    Py_ssize_t i, size;
    
    // Parse arguments - expect a list
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &list))
        return NULL;
    
    // Get the size of the list
    size = PyList_Size(list);
    
    // Check if list is empty
    if (size == 0) {
        PyErr_SetString(PyExc_ValueError, "Cannot find mode of empty list");
        return NULL;
    }
    
    // Create a dictionary to count occurrences
    count_dict = PyDict_New();
    if (!count_dict)
        return NULL;
    
    // Count occurrences of each value
    for (i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(list, i);
        PyObject *count_obj;
        int count;
        
        // Check if item is numeric
        if (!PyFloat_Check(item) && !PyLong_Check(item)) {
            Py_DECREF(count_dict);
            PyErr_SetString(PyExc_TypeError, "List must contain only numbers");
            return NULL;
        }
        
        // Get current count for this value
        count_obj = PyDict_GetItem(count_dict, item);
        if (count_obj) {
            count = PyLong_AsLong(count_obj) + 1;
        } else {
            count = 1;
        }
        
        // Update the count
        PyDict_SetItem(count_dict, item, PyLong_FromLong(count));
        
        // Check if this is the new mode
        if (count > max_count) {
            max_count = count;
            Py_XDECREF(mode);
            mode = item;
            Py_INCREF(mode);
        }
    }
    
    Py_DECREF(count_dict);
    
    if (!mode) {
        Py_RETURN_NONE;
    }
    
    return mode;
}

// Function to count the number of values in the array
static PyObject *
stat_module_count_array(PyObject *self, PyObject *args)
{
    PyObject *list;
    Py_ssize_t size;
    
    // Parse arguments - expect a list
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &list))
        return NULL;
    
    // Get the size of the list
    size = PyList_Size(list);
    
    return PyLong_FromSsize_t(size);
}

// Method table
static PyMethodDef StatModuleMethods[] = {
    {"sum_array", stat_module_sum_array, METH_VARARGS, "Calculate sum of values in an array"},
    {"avg_array", stat_module_avg_array, METH_VARARGS, "Calculate average of values in an array"},
    {"std_dev_array", stat_module_std_dev_array, METH_VARARGS, "Calculate standard deviation of values in an array"},
    {"mode_array", stat_module_mode_array, METH_VARARGS, "Find mode (most frequent value) in an array"},
    {"count_array", stat_module_count_array, METH_VARARGS, "Count number of values in an array"},
    {NULL, NULL, 0, NULL}  // Sentinel
};

// Module definition
static struct PyModuleDef statmodule = {
    PyModuleDef_HEAD_INIT,
    "stat_module",      // Module name
    "A module providing statistical functions for arrays", // Module documentation
    -1,           // Size of per-interpreter state or -1
    StatModuleMethods  // Method table
};

// Module initialization function
PyMODINIT_FUNC
PyInit_stat_module(void)
{
    return PyModule_Create(&statmodule);
}
