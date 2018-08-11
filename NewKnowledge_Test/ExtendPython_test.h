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
	// ��ʼ��Python
	// ��ʹ��Pythonϵͳǰ������ʹ��Py_Initialize������г�ʼ������������Python���ڽ�ģ��
	// �����ϵͳ·����ģ������·���С�����Ƿ��ʼ���ɹ���Ҫ����Py_IsInitialized

	if (!InitPyEnv()) return;

	PyRun_SimpleString("from time import time,ctime\n"
		"print 'Today is',ctime(time())\n");

	PyRun_SimpleString("import os\n"
		"print os.getcwd()\n");
	// ��ӵ�ǰ·��  pyrun_simplestring �ӿ�0�ɹ�
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");

	PyRun_SimpleString("print sys.path");

	PyObject *pName, *pModule, *pDict, *pFunc, *pArgs;

	// ���ؽű�
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

	// ���Һ���
	pFunc = PyDict_GetItemString(pDict, "add");
	if (!pFunc || !PyCallable_Check(pFunc))
	{
		cout << "PyDict_GetItemString Or PyCallable_Check Faild" << endl;
		return;
	}

	// ������ջ
	pArgs = PyTuple_New(2);

	//  PyObject* Py_BuildValue(char *format, ...)
	//  ��C++�ı���ת����һ��Python���󡣵���Ҫ��
	//  C++���ݱ�����Pythonʱ���ͻ�ʹ������������˺���
	//  �е�����C��printf������ʽ��ͬ�����õĸ�ʽ��
	//  s ��ʾ�ַ�����
	//  i ��ʾ���ͱ�����
	//  f ��ʾ��������
	//  O ��ʾһ��Python����
	
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("l", 3));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("l", 4));

	// ����python����
	PyObject_CallObject(pFunc, pArgs);

	//Py_DecRef(pName);
	Py_DecRef(pArgs);
	Py_DecRef(pModule);

	UnitPyEnv();


}