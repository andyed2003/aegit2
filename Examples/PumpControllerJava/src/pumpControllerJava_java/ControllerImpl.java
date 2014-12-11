package pumpControllerJava_java;

import static pumpControllerJava_java.MainEntry.*;

// Task: ControllerImpl

public class ControllerImpl implements Runnable {

	// Instance variables and constants
	protected int c_level = 100;
	protected boolean c_pumpOnReq = false;
	protected boolean c_pumpOnCmd = false;
	protected boolean c_warn = false;
	protected int c_level_internal = 100;
	protected boolean c_pumpOnReq_internal = false;
	protected boolean c_pumpOnCmd_internal = false;
	protected boolean c_warn_internal = false;
	protected boolean commit = false;
	protected int priority = 5;

	public ControllerImpl() {
	}

	public void run() {
		while (true) {
			// [Internal] Timer information for repeating or periodic tasks
			long THREAD_START_TIME = System.currentTimeMillis();

			// Translated code
			Pointer<Integer> c_levelPointer = new Pointer<Integer>();
			((EnvironmentImpl) MainEntry.getTask("EnvironmentImpl"))
					.getLevel_eAPI(c_levelPointer);
			c_level = c_levelPointer.value;
			Pointer<Boolean> c_pumpOnReqPointer = new Pointer<Boolean>();
			((EnvironmentImpl) MainEntry.getTask("EnvironmentImpl"))
					.getPumpOnReq_eAPI(c_pumpOnReqPointer);
			c_pumpOnReq = c_pumpOnReqPointer.value;
			c_level_internal = c_level;
			c_pumpOnReq_internal = c_pumpOnReq;
			if ((c_level_internal <= MIN) && (c_pumpOnReq_internal == true)) {
				c_warn_internal = true;
				commit = true;
			} else {
				c_pumpOnCmd_internal = true;
				commit = true;
			}
			c_warn = c_warn_internal;
			c_pumpOnCmd = c_pumpOnCmd_internal;
			((EnvironmentImpl) MainEntry.getTask("EnvironmentImpl"))
					.setPumpOnCmd_eAPI(c_pumpOnCmd);
			((EnvironmentImpl) MainEntry.getTask("EnvironmentImpl"))
					.setWarn_eAPI(c_warn);

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
	public int getPriority() {
		return priority;
	}

}
