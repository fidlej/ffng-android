package cz.ger.ffng;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.HashSet;

import android.util.Log;

import com.badlogic.gdx.Files.FileType;
import com.badlogic.gdx.files.FileHandle;

public class FFNGFiles {
	public final static int INTERNAL = 1;
	public final static int EXTERNAL = 2;
	public final static int ABSOLUTE = 3;
	
	protected static boolean useCache = false;
	// Vector, HashSet, ArrayList, LinkedList
	protected static final HashSet<String> internalFileList = new HashSet<String>();
	protected static final HashSet<String> externalFileList = new HashSet<String>();
	
	protected static final String sdcardStorage = "Android/data/cz.ger.ffng";
	
	public void dummy() {}
	
	public static void createCache() {
		//createFileList(internalFileList, getFileHandle("", FileType.Internal));	// too slow :(
		readFileList(internalFileList, "filelist.txt");
		createFileList(externalFileList, FFNG.files.getFileHandle(sdcardStorage, FileType.External));
		useCache = true;
	}
	
	protected static void readFileList(HashSet<String> fileList, String sourceFile) {
		String list;
		try {
			list = readStream(FFNG.files.getFileHandle(sourceFile, FileType.Internal).read());
		} catch (IOException e) {
			Log.e("FFNG", "Unable to load " + sourceFile + " to cache the internal file list");
			list = "";
		}
		String[] splittedList = list.split("\n");
		for (String s : splittedList) {
			if (s.trim() != "") {
				fileList.add(s.trim());
			}
		}
	}
	
	protected static void createFileList(HashSet<String> fileList, FileHandle fh) {
		if (fh == null) return;
		FileHandle[] list = fh.list();
		
		for (FileHandle f : list) {
			if (f.isDirectory()) {
				createFileList(fileList, f);
			} else {
				fileList.add(f.path());  // TODO is this concatenation right?
			}
		}
	}
	
	static public FileType getFileType(int type) {
		FileType ft = FileType.Internal;
		switch(type) {
			case ABSOLUTE: ft = FileType.Absolute; break;
			case EXTERNAL: ft = FileType.External; break;
			case INTERNAL:
			default: ft = FileType.Internal; break;
		}
		return ft;
	}
	
	static public String correctPath(String path, FileType type) {
		if (type == FileType.External) {
			return sdcardStorage + "/" + path;
		} else {
			return path;
		}
	}
	
	static public FileHandle getFileHandle(String file, int type) throws IOException {
		FileType ft = getFileType(type);
		
		FileHandle fh = null;
		try {
			fh = FFNG.files.getFileHandle(correctPath(file, ft), ft);
		} catch(Exception e) {
			throw new IOException("File doesn't exist: " + file + " (" + (type == ABSOLUTE ? "absolute" : (type == EXTERNAL ? "external" : "internal")));
		} catch(NoSuchMethodError e) {
			Log.d("FFNG", "FFNGFiles" + e.getMessage());
		}
		
		return fh;
	}
	
	static public boolean exists(String path, int type) {
		/*
		try {
			getFileHandle(path, type).read();
		} catch(Exception ex) {
			return false;
		}
		return true;
		*/
		if (useCache && (type == INTERNAL || type == EXTERNAL)) {
			switch (type) {
				case INTERNAL: return internalFileList.contains(path);
				case EXTERNAL: return externalFileList.contains(FFNG.files.getExternalStoragePath() + sdcardStorage + "/" + path);
			}
		}
		try {
			return getFileHandle(path, type).exists();
		} catch(Exception ex) {
			return false;
		}
	}
	
	static public String read(String path, int type) {
		Log.d("FFNG", "FFNGFiles::read " + path);
		try {
			return readStream(getFileHandle(path, type).read());
		} catch(Exception ex) {
			return "";
		}
	}
	
	static public String readStream(InputStream is) throws IOException {
	    /*
	     * To convert the InputStream to String we use the
	     * Reader.read(char[] buffer) method. We iterate until the
	     * Reader return -1 which means there's no more data to
	     * read. We use the StringWriter class to produce the string.
	     */
	    if (is != null) {
	        Writer writer = new StringWriter();
	
	        char[] buffer = new char[1024];
	        try {
	            Reader reader = new BufferedReader(new InputStreamReader(is, "UTF-8"), 1024);
	            int n;
	            while ((n = reader.read(buffer)) != -1) {
	                writer.write(buffer, 0, n);
	            }
	        } catch(Exception ex) {
	        	return "";
	        } finally {
	        	try {
	        		is.close();
	        	} catch(Exception ex) {
	        	}
	        }
	        return writer.toString();
	    } else {        
	        return "";
	    }
	}
	
	public static void createPath(String path) {	// external
		FFNG.files.external(sdcardStorage + "/" + path).parent().mkdirs();
	}
	
	public static boolean write(String path, String data) {
		OutputStream os = FFNG.files.external(sdcardStorage + "/" + path).write(false);
		
		try {
			os.write(data.getBytes("UTF-8"));
			externalFileList.add(FFNG.files.getExternalStoragePath() + sdcardStorage + "/" + path);
		} catch (IOException e) {
			Log.e("FFNG", "error writing file " + path + ": " + e.getMessage());
			return false;
		}
		return true;
	}
}
