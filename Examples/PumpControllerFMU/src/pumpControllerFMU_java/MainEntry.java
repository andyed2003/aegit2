// MainEntry: MainEntry
package pumpControllerFMU_java;

import java.util.HashMap;

public class MainEntry {
	protected static final int MIN = 10;

	//	Stores all tasks and shared machines with their names
	private static HashMap<String, Object> tasks = new HashMap<String, Object>();

	public static void addTask(Object task, String name) {
		tasks.put(name, task);
	}

	public static Object getTask(String name) {
		return tasks.get(name);
	}

	public static void main(String[] args) {
		// Create the tasks
		ControllerImpl task0 = new ControllerImpl();
		EnvironmentImpl task1 = new EnvironmentImpl();

		// Store the tasks in a shared map to allow communication between tasks
		addTask(task0, "ControllerImpl");
		addTask(task1, "EnvironmentImpl");

		// Create the threads
		Thread thread0 = new Thread(task0);
		Thread thread1 = new Thread(task1);

		// Set priority
		thread0.setPriority(task0.getPriority());
		thread1.setPriority(task1.getPriority());

		// Start the threads
		thread0.start();
		thread1.start();
	}
}
