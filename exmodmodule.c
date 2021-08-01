#include <python3.7m/Python.h>

static PyObject *exmodError;

static PyObject* exmod_say_hello(PyObject* self, PyObject *args) {
    const char* msg;
    int sts=0;

    if (!PyArg_ParseTuple(args, "s", &msg)) {
        return NULL; // return error if none found
    }

    // Check to see if there is any problem witht he inform passed form python to this module
    if (strcmp(msg, "this_is_an_error") == 0) {
        PyErr_SetString(exmodError, "This is a test exception error after typung an expected invalid response");
        return NULL;

    } else {
        // No error found
        printf("This is C world\n You message is: %s\n", msg);
        sts = 21;
    }

    return Py_BuildValue("i", sts);
}

static PyObject* exmod_add(PyObject* self, PyObject *args){
    double a,b;
    double sts=0;
    // We expect at least 1 string argument to this function
    if (!PyArg_ParseTuple(args, "dd", &a,&b))
    {
        return NULL;
    }

    sts = a + b;
    printf("This is C world\n Addition of %f + %f = %f", a,b,sts);
    return Py_BuildValue("i", sts);
}

static PyMethodDef exomod_methods[] = {
    {"say_hello", exmod_say_hello, METH_VARARGS, "Say hello from C and print message"},
    {"add", exmod_add, METH_VARARGS, "Add two numbers in C"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef exmod = {
    PyModuleDef_HEAD_INIT,
    "exmod",
    NULL,
    -1,
    exomod_methods
};

PyMODINIT_FUNC PyInit_exmod(void)
{
    PyObject *m;
    m = PyModule_Create(&exmod);
    if(m == NULL) return;

    exmodError = PyErr_NewException("exmod.error", NULL, NULL);
    Py_INCREF(exmodError);
    if (PyModule_AddObject(m, "error", exmodError) <0) {
        Py_XDECREF(exmodError);
        Py_CLEAR(exmodError);
        Py_DECREF(m);
    }

    return(m);
}
