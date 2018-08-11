#include "python.h"

//#ifdef DEBUG
//#pragma comment(lib, "python27_d.lib")
//#else
//#pragma comment(lib, "python27.lib")
//#endif // DEBUG


bool InitPyEnv()
{
	Py_Initialize();

	return 0 != Py_IsInitialized();
}

void UnitPyEnv()
{
	Py_Finalize();
}

void UsePythonBuildinType_test()
{
	if (!InitPyEnv()) return;

	PyObject* pInt = Py_BuildValue("i", 2015);
	assert(PyInt_Check(pInt));
	int i = PyInt_AsLong(pInt);

	UnitPyEnv();
}

void printDict(PyObject* obj) {

	if (!PyDict_Check(obj))
		return;
	PyObject *k, *keys;
	keys = PyDict_Keys(obj);
	for (int i = 0; i < PyList_GET_SIZE(keys); i++) {
		k = PyList_GET_ITEM(keys, i);
		char* c_name = PyString_AsString(k);
		printf("%s\n", c_name);
	}
}

void RunPythonFile_Test()
{
	// 初始化Python
	// 在使用Python系统前，必须使用Py_Initialize对其进行初始化，将会载入Python的内建模块
	// 并添加系统路径到模块搜索路径中。检测是否初始化成功需要调用Py_IsInitialized

	if (!InitPyEnv()) return;

	PyRun_SimpleString("from time import time,ctime\n"
		"print 'Today is',ctime(time())\n");

	PyRun_SimpleString("import os\n"
		"print os.getcwd()\n");
	// 添加当前路径  pyrun_simplestring 接口0成功
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");

	PyRun_SimpleString("print sys.path");

	PyObject *pName, *pModule, *pDict, *pFunc, *pArgs;

	// 加载脚本
	pName = PyString_FromString("test");
	pModule = PyImport_Import(pName);
	if (!pModule)
	{
		cout << "can't find test.py" << endl;
		getchar();
		return;
	}

	pDict = PyModule_GetDict(pModule);
	if (!pDict)
	{
		cout << "PyModule_GetDict return null" << endl;
		return;
	}

	printDict(pDict);

	// 查找函数
	pFunc = PyDict_GetItemString(pDict, "add");
	if (!pFunc || !PyCallable_Check(pFunc))
	{
		cout << "PyDict_GetItemString Or PyCallable_Check Faild" << endl;
		return;
	}

	// 参数进栈
	pArgs = PyTuple_New(2);

	//  PyObject* Py_BuildValue(char *format, ...)
	//  把C++的变量转换成一个Python对象。当需要从
	//  C++传递变量到Python时，就会使用这个函数。此函数
	//  有点类似C的printf，但格式不同。常用的格式有
	//  s 表示字符串，
	//  i 表示整型变量，
	//  f 表示浮点数，
	//  O 表示一个Python对象。
	
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("l", 3));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("l", 4));

	// 调用python函数
	PyObject_CallObject(pFunc, pArgs);

	//Py_DecRef(pName);
	Py_DecRef(pArgs);
	Py_DecRef(pModule);

	UnitPyEnv();


}