import java.io.*;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class ClientHandler extends Thread {

    private Socket socket;
    private InputStream in;
    private OutputStream out;
    private byte[] bytes = new byte[1024];
    private String string;
    private String filename;
    private long filesize;
    private File file;
    private boolean isWorkEnd = false;

    public ClientHandler(Socket socket) throws IOException {
        this.socket = socket;
        in = socket.getInputStream();
        out = socket.getOutputStream();
    }

    @Override
    public void run() {
        System.out.println("Server has started");
        File dir = new File("uploads");
        if (!dir.exists()) {
            dir.mkdir();
        }
        try {
            // Read file parameters
            int count = in.read(bytes);
            string = new String(bytes, 0, count, StandardCharsets.UTF_8);
            System.out.println(string);
            Arrays.fill(bytes, (byte) 0);

            // Answer to client that parameters accepted
            out.write((string + "\n").getBytes(StandardCharsets.UTF_8));
            out.flush();

            // Creating file ...
            filename = string.substring(0, string.indexOf(' '));
            filesize = Long.parseLong(string.substring(string.indexOf(' ') + 1, string.indexOf('\n')));
            file = new File("uploads/" + filename);

            if (file.createNewFile()) {
                System.out.println(filename + " was created");
            }

            try (FileOutputStream fio = new FileOutputStream(file)) {

                // Read file ...
                // TODO create every 3 sec stdout speed info
                while ((count = in.read(bytes)) > 0) {
                    if (bytes[count - 1] == -1) {
                        fio.write(bytes, 0, count - 1);
                        break;
                    }
                    fio.write(bytes, 0, count);
                    Arrays.fill(bytes, (byte) 0);// получив - выводим на экран
                }

                if (file.length() == filesize) {
                    out.write(("File " + file.getName() + "uploaded successfully").getBytes(StandardCharsets.UTF_8));
                    out.flush();
                } else {
                    out.write(("File " + file.getName() + "uploaded unsuccessfully [" + file.length() + "/" + filesize + "]").getBytes(StandardCharsets.UTF_8));
                    out.flush();
                }
                isWorkEnd = true;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void close() {
        try {
            out.close();
            in.close();
            socket.close();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    public boolean isWorkEnd() {
        return isWorkEnd;
    }
}
