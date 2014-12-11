package pumpControllerJava_java;

public interface IEnvironmentImpl {
	public void getLevel_eAPI(Pointer<Integer> p1);

	public void getPumpOnReq_eAPI(Pointer<Boolean> p2);

	public void setPumpOnCmd_eAPI(boolean p3);

	public void setWarn_eAPI(boolean p4);
}
