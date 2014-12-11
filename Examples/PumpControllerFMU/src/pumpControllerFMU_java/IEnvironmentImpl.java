package pumpControllerFMU_java;

public interface IEnvironmentImpl {
	public void c_read(Pointer<Integer> p1, Pointer<Boolean> p2);

	public void c_write(boolean p3, boolean p4);
}
