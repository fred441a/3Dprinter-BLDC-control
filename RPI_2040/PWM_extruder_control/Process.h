#pragma once
class Process {
private:
	void func();//!< The function the process runs

public:
	/**
	 * @param func a pointer to the function the process runs(this might have to be mem_cpy to func())
	 */
  Process(void *func);
};
