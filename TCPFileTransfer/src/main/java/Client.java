import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class Client {

    private static final int FILENAME_INDEX = 0;
    private static final int HOST_IP_INDEX = 1;
    private static final int HOST_PORT_INDEX = 2;

    private static String filename;
    private static String hostIP;
    private static int hostPort;
    private static File fileToTransfer;
    private static long filesize;
    private static byte[] bytes = new byte[10];

    public static void main(String[] args) {
        if(args.length < 3) {
            System.err.println("Too few arguments, hint [first] - filename to transfer, [second] - host ip, [third] - host port");
        }

        filename = args[FILENAME_INDEX];
        hostIP = args[HOST_IP_INDEX];
        hostPort = Integer.parseInt(args[HOST_PORT_INDEX]);

        fileToTransfer = new File(filename);
        if(!fileToTransfer.exists()) {
            System.err.println("File not exists, try to input other file");
        }
        filesize = fileToTransfer.length();

        try (Socket clientSocket = new Socket(hostIP, hostPort);
             FileInputStream fileInputStream = new FileInputStream(fileToTransfer);
             // читать соообщения с сервера
             InputStream in = clientSocket.getInputStream();
            // писать туда же
             OutputStream out = clientSocket.getOutputStream()
            ) {
                System.out.println("Client is started");
                // если соединение произошло и потоки успешно созданы - мы можем
                //  работать дальше

                out.write((fileToTransfer.getName() + "\n" + filesize + "\n").getBytes()); // отправляем сообщение на сервер
                out.flush();
                int count;
                while ((count = in.read(bytes)) > 0) {
                    System.out.print(new String(bytes, 0, count, StandardCharsets.UTF_8));
                    Arrays.fill(bytes, (byte) 0);// получив - выводим на экран
                }
            // ждём, что скажет сервер
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}