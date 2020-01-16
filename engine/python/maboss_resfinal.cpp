#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <fstream>
#include <stdlib.h>
#include <set>
#include "src/BooleanNetwork.h"
#include "src/FinalStateSimulationEngine.h"

typedef struct {
  PyObject_HEAD
  Network* network;
  FinalStateSimulationEngine* engine;
} cMaBoSSResultFinalObject;

static void cMaBoSSResultFinal_dealloc(cMaBoSSResultFinalObject *self)
{
    free(self->engine);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject * cMaBoSSResultFinal_new(PyTypeObject* type, PyObject *args, PyObject* kwargs) 
{
  cMaBoSSResultFinalObject* res;
  res = (cMaBoSSResultFinalObject *) type->tp_alloc(type, 0);

  return (PyObject*) res;
}

static PyObject* cMaBoSSResultFinal_get_last_states_probtraj(cMaBoSSResultFinalObject* self) {
  
  PyObject *dict = PyDict_New();
  
  // Building the results as a python dict
  for (auto& result : self->engine->getFinalStates()) {
    PyDict_SetItem(
      dict, 
      PyUnicode_FromString(NetworkState(result.first).getName(self->network).c_str()), 
      PyFloat_FromDouble(result.second)
    );
  }
  return (PyObject*) dict;
}

// static PyObject* cMaBoSSResult_get_last_nodes_probtraj(cMaBoSSResultFinalObject* self) {
  
//   PyObject *dict = PyDict_New();
  
//   // Building the results as a python dict
//   for (auto& result : self->engine->getFinalStates()) {
//     PyDict_SetItem(
//       dict, 
//       PyUnicode_FromString(result.first->getLabel().c_str()), 
//       PyFloat_FromDouble(result.second)
//     );
//   }
//   return (PyObject*) dict;
// }


static PyMethodDef cMaBoSSResultFinal_methods[] = {
    {"get_last_states_probtraj", (PyCFunction) cMaBoSSResultFinal_get_last_states_probtraj, METH_NOARGS, "gets the last probtraj of the simulation"},
    // {"get_last_nodes_probtraj", (PyCFunction) cMaBoSSResultFinal_get_last_nodes_probtraj, METH_NOARGS, "gets the last nodes probtraj of the simulation"},
    {NULL}  /* Sentinel */
};

static PyTypeObject cMaBoSSResultFinal = []{
  PyTypeObject res{PyVarObject_HEAD_INIT(NULL, 0)};

  res.tp_name = "cmaboss.cMaBoSSResultFinalObject";
  res.tp_basicsize = sizeof(cMaBoSSResultFinalObject);
  res.tp_itemsize = 0;
  res.tp_flags = Py_TPFLAGS_DEFAULT;// | Py_TPFLAGS_BASETYPE;
  res.tp_doc = "cMaBoSSResultFinalObject";
  res.tp_new = cMaBoSSResultFinal_new;
  res.tp_dealloc = (destructor) cMaBoSSResultFinal_dealloc;
  res.tp_methods = cMaBoSSResultFinal_methods;
  return res;
}();
