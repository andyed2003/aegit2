package pumpControllerFMU_java;

import static pumpControllerFMU_java.MainEntry.*;

// EnvironTask: EnvironmentImpl

public class EnvironmentImpl implements Runnable, IEnvironmentImpl {

	// Instance variables and constants
	protected int e_level = 30;
	protected boolean e_pumpOnReq = false;
	protected boolean e_pumpOnCmd = false;
	protected boolean e_warn = false;
	protected boolean init;
	private static Object lock = new Object();

	protected int priority = 5;

	public EnvironmentImpl() {
	}

	public void run() {
		while (true) {
			// [Internal] Timer information for repeating or periodic tasks
			long THREAD_START_TIME = System.currentTimeMillis();
			synchronized (lock) {
				// Translated code
				if ((e_level < 20)) {
					e_level = ((e_level) - 1);
					e_pumpOnReq = true;
				} else {
					e_level = ((e_level) - 7);
					e_pumpOnReq = false;
				}
				e_level = e_level;
			}
			// [Internal] Code to monitor time between periodic tasks
			long THREAD_END_TIME = System.currentTimeMillis();
			long THREAD_TIME_TAKEN = THREAD_END_TIME - THREAD_START_TIME;
			try {
				Thread.sleep(Math.max(0 - THREAD_TIME_TAKEN, 0));
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	// Subroutines
	public void c_read(Pointer<Integer> p1, Pointer<Boolean> p2) {
		// [Internal] This will block until the guard has been met
		boolean completed = false; // private to the thread by default
		// [Internal] Try and get lock
		synchronized (lock) {
			// Translated code
			p1.value = e_level;
			p2.value = e_pumpOnReq;
			// [Internal] Set completed flag
			completed = true;
		}
	}

	public void c_write(boolean p3, boolean p4) {
		// [Internal] This will block until the guard has been met
		boolean completed = false; // private to the thread by default
		// [Internal] Try and get lock
		synchronized (lock) {
			// Translated code
			e_pumpOnCmd = p3;
			e_warn = p4;
			// [Internal] Set completed flag
			completed = true;
		}
	}

	public int getPriority() {
		return priority;
	}

}
