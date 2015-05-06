#include "Python.h"
#include <stdio.h>
#include "../diskco/diskco.h"
static PyObject *ErrorObject;

typedef struct {
  PyObject_HEAD
  PyObject            *x_attr;        /* Attributes dictionary */
  Diskco* parent;
} DiskcoObject;

 


/* Diskco methods */

static void
Diskco_dealloc(DiskcoObject *self)
{
  delete self->parent;
  Py_XDECREF(self->x_attr);
  PyObject_Del(self);
}

static PyObject *
Diskco_copy(DiskcoObject *self, PyObject *args)
{
  try {
    int64_t offset, length;
    if (!PyArg_ParseTuple(args, "LL:copy", &offset, &length))
      return NULL;
    self->parent->copy(offset, length);
    Py_INCREF(Py_None);
    return Py_None;
  }catch (std::runtime_error e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
}

static PyObject *
Diskco_close(DiskcoObject *self, PyObject *args)
{
  try{
    if (!PyArg_ParseTuple(args, ":close"))
      return NULL;
    self->parent->close();
    Py_INCREF(Py_None);
    return Py_None;
  }catch (std::runtime_error e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
}

static PyObject *
Diskco_search(DiskcoObject *self, PyObject *args)
{
  int64_t segment_offset, segment_length, offset, length;
  char* search_bytes;
  int search_length;
  try{
    if (!PyArg_ParseTuple(args, "s#LLLL:search", &search_bytes, &search_length, &offset, &length, &segment_offset, &segment_length))
      return NULL;
    self->parent->set_search(std::string(search_bytes, search_length), offset, length, segment_offset, segment_length);
    Py_INCREF(Py_None);
    return Py_None;
  }catch (std::runtime_error e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
}

static PyObject *
Diskco_next_buffer(DiskcoObject *self, PyObject *args)
{
  Buffer* result;
  try{
    if (!PyArg_ParseTuple(args, ":next_buffer"))
      return NULL;
    result = self->parent->next_buffer();
    if (result == NULL) {
      Py_INCREF(Py_None);
      return Py_None;
    }
    PyObject *tuple = Py_BuildValue("(y#L)", result->buffer(), result->size(), result->source_offset());
    self->parent->release_buffer(result);
    return tuple;
  }catch (std::runtime_error e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
}

static PyMethodDef Diskco_methods[] = {
  {"copy",            (PyCFunction)Diskco_copy,  METH_VARARGS,
    PyDoc_STR("copy(offset, length) -> Copy bytes from a certain offset")},
  {"search",             (PyCFunction)Diskco_search,  METH_VARARGS,
    PyDoc_STR("search(offset, length, pattern, segment_offset, segment_length) -> Configure object to search the input, for every match copy <segment_length> bytes at <segment_offset> bytes from the match.")},
  {"next_buffer",             (PyCFunction)Diskco_next_buffer,  METH_VARARGS,
    PyDoc_STR("next_buffer() -> returns processed data one block at a time.")},


  {"close",           (PyCFunction)Diskco_close, METH_VARARGS,
    PyDoc_STR("close() -> Close the files")},
  {NULL,              NULL}           /* sentinel */
};


static PyObject *
Diskco_getattro(DiskcoObject *self, PyObject *name)
{
  if (self->x_attr != NULL) {
    PyObject *v = PyDict_GetItem(self->x_attr, name);
    if (v != NULL) {
      Py_INCREF(v);
      return v;
    }
  }
  return PyObject_GenericGetAttr((PyObject *)self, name); 
}

static int
Diskco_setattr(DiskcoObject *self, char *name, PyObject *v)
{
  if (self->x_attr == NULL) {
    self->x_attr = PyDict_New();
    if (self->x_attr == NULL)
      return -1;
  }
  if (v == NULL) {
    int rv = PyDict_DelItemString(self->x_attr, name);
    if (rv < 0)
      PyErr_SetString(PyExc_AttributeError,
                      "delete non-existing Diskco attribute");
    return rv;
  }
  else
    return PyDict_SetItemString(self->x_attr, name, v);
 
 
}

static PyTypeObject Diskco_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "diskco.Diskco",             /*tp_name*/
  sizeof(DiskcoObject),          /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)Diskco_dealloc,    /*tp_dealloc*/
  0,                          /*tp_print*/
  (getattrfunc)0,             /*tp_getattr*/
  (setattrfunc)Diskco_setattr,   /*tp_setattr*/
  0,                          /*tp_reserved*/
  0,                          /*tp_repr*/
  0,                          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash*/
  0,                          /*tp_call*/
  0,                          /*tp_str*/
  (getattrofunc)Diskco_getattro, /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  0,                          /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  Diskco_methods,                /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  0,                          /*tp_init*/
  0,                          /*tp_alloc*/
  0,                          /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};
/* --------------------------------------------------------------------- */

#define DiskcoObject_Check(v)      (Py_TYPE(v) == &Diskco_Type)

static DiskcoObject *
newDiskcoObject(char* input, char* output, bool append, bool swap_bytes)
{
  try {
    DiskcoObject *self;
    self = PyObject_New(DiskcoObject, &Diskco_Type);
    if (self == NULL)
      return NULL;
    self->parent = new Diskco(input, output, append, swap_bytes);
    self->x_attr = NULL;
    return self;
  }catch (std::runtime_error e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
} 


/* Function of no arguments returning new Diskco object */

static PyObject *
diskco_new(PyObject *self, PyObject *args, PyObject* keywds)
{
  DiskcoObject *rv;
  int append = 0;
  int swap_bytes = 0;
  char* input, *output;
  static char *kwlist[] = {(char*)"input", (char*)"output",(char*)"append", (char*)"byteswap",NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss|$pp:new", kwlist, &input, &output, &append, &swap_bytes))
    return NULL;
  rv = newDiskcoObject(input, output, append, swap_bytes);
  if (rv == NULL)
    return NULL;
  return (PyObject *)rv;
}

static PyObject *
diskco_copy_file(PyObject *self, PyObject *args, PyObject* keywds)
{
  try {
    int append = 0;
    int swap_bytes = 0;
    char *input, *output;
    int64_t offset, length;
    static char *kwlist[] = {(char*)"input", (char*)"offset", (char*)"length", (char*)"output",(char*)"append", (char*)"byteswap",NULL};
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "sLLs|$pp:copy_file", kwlist, &input, &offset, &length, &output, &append, &swap_bytes))
      return NULL;
  
    Diskco* diskco = new Diskco(input, output, append, swap_bytes);
    diskco->copy(offset, length);
    diskco->close();
    delete diskco;
    Py_INCREF(Py_None);
    return Py_None;
  }catch (std::runtime_error e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
}

/* List of functions defined in the module */

static PyMethodDef diskco_methods[] = {
  {"Diskco",             (PyCFunction)diskco_new,         METH_VARARGS|METH_KEYWORDS,
    PyDoc_STR("Diskco(source, destination, append=False, byteswap=False) -> new Diskco object")},
  {"copy_file",         (PyCFunction)diskco_copy_file,         METH_VARARGS|METH_KEYWORDS,
    PyDoc_STR("copy_file(source, offset, length, destination, byteswap=False, append=False) -> Copy data from source to destination")},
  
  {NULL,              NULL}           /* sentinel */
};

PyDoc_STRVAR(module_doc,
             "This module provides fast dd functionality in python.");



static struct PyModuleDef diskcomodule = {
  PyModuleDef_HEAD_INIT,
  "diskco",
  module_doc,
  -1,
  diskco_methods,
  NULL,
  NULL,
  NULL,
  NULL
};

PyMODINIT_FUNC
PyInit_diskco(void)
{
  PyObject *m = NULL;
  
  
  /* Finalize the type object including setting type of the new type
   * object; doing it here is required for portability, too. */
  if (PyType_Ready(&Diskco_Type) < 0)
    goto fail;
  
  /* Create the module and add the functions */
  m = PyModule_Create(&diskcomodule);
  if (m == NULL)
    goto fail;
  
  /* Add some symbolic constants to the module */
  if (ErrorObject == NULL) {
    ErrorObject = PyErr_NewException("diskco.error", NULL, NULL);
    if (ErrorObject == NULL)
      goto fail;
  }
  Py_INCREF(ErrorObject);
  PyModule_AddObject(m, "error", ErrorObject);
  
  return m;
fail:
  Py_XDECREF(m);
  return NULL;
}
