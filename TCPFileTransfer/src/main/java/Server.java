import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class Server {
    private static byte[] bytes = new byte[1024];
    private static String string;
    private static String filename;
    private static long filesize;
    private static File file;

    public static void main(String[] args) {
        try(ServerSocket server = new ServerSocket(4004);
            Socket clientSocket = server.accept();
            InputStream in = clientSocket.getInputStream();
            OutputStream out = clientSocket.getOutputStream()
            ) {
                System.out.println("Server has started");
                File dir = new File("uploads");
                if(!dir.exists()) {
                    dir.mkdir();
                }

                // Read file parameters
                int count = in.read(bytes);
                string = new String(bytes, 0, count, StandardCharsets.UTF_8);
                System.out.println(string);
                Arrays.fill(bytes, (byte) 0);

                // Answer to client that parameters accepted
                out.write(("Hello, it's Server! confirm,that you have written : " + string + "\n").getBytes(StandardCharsets.UTF_8));
                out.flush();

                // Creating file ...
                filename = string.substring(0, string.indexOf(' '));
                filesize = Long.parseLong(string.substring(string.indexOf(' ') + 1, string.indexOf('\n')));
                file = new File("uploads/" + filename);

                if(file.createNewFile()) {
                    System.out.println(filename + " was created");
                }

                try(FileOutputStream fio = new FileOutputStream(file)){

                // Read file ...
                while ((count = in.read(bytes)) > 0) {
                    if (bytes[count - 1] == -1) {
                        break;
                    }
                    fio.write(bytes, 0, count);
                    Arrays.fill(bytes, (byte) 0);// получив - выводим на экран
                }

                if (file.length() == filesize) {
                    out.write(("File uploaded successfully").getBytes(StandardCharsets.UTF_8));
                    out.flush();
                }

            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}