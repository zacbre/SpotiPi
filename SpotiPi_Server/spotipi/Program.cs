using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace spotipi
{
    class Program
    {
        public static SpotySong song = new SpotySong();
        static void Main(string[] args)
        {
            spotify m = new spotify();
            string songname = null;
            //listen for HTTP requests.
            new Thread(new ThreadStart(delegate()
            {
                Socket listen1 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                listen1.Bind(new IPEndPoint(IPAddress.Any, 5555));
                listen1.Listen(1000);
                Console.WriteLine("Listening on port 5555.");
                while (true)
                    new Thread(new ParameterizedThreadStart(PortHandler)).Start(listen1.Accept());

            })).Start();
            while (true)
            {
                if (m.Nowplaying() == "Spotify")
                {
                    song.artist = "";
                    song.songtitle = "Not Playing.";
                    Thread.Sleep(0);
                    continue;
                }
                if(songname == null || songname != m.Nowplaying().Replace("Spotify - ", ""))
                {
                    songname = m.Nowplaying().Replace("Spotify - ", "");
                    string[] x = m.Nowplaying().Replace("Spotify - ", "").Split('–');
                    song.songtitle = x[1].Substring(1, x[1].Length - 1);
                    song.artist = x[0].Substring(0, x[0].Length - 1);
                    Thread.Sleep(0);
                }
                else
                {
                    Thread.Sleep(0); continue;
                }
            }
        }

        static public void PortHandler(object _socket)
        {
            Socket client = (Socket)_socket;
            string request = "";
            try
            {
                byte[] buffer = new byte[1024];
                int got = client.Receive(buffer);
                if (got < 0)
                {
                    try
                    {
                        client.Close();
                    }
                    catch { }
                    return;
                }
                request = ASCIIEncoding.ASCII.GetString(buffer, 0, got).Trim();
            }
            catch
            {
                try
                {
                    client.Close();
                }
                catch { }
                return;
            }
            //return last two messages.
            string lastshit = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keepalive\r\nKeep-Alive: 300\r\nContent-Length: {0}\r\n\r\n{1}";
            //send shit to client.
            string output = song.artist + "\n" + song.songtitle;
            client.Send(Encoding.ASCII.GetBytes(string.Format(lastshit, output.Length, output)));
            client.Close();
            return;
        }
    }
    public class SpotySong
    {
        public string songtitle { get; set; }
        public string artist { get; set; }
    }
    public class spotify
    {
        [DllImport("USER32.DLL")]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);
        [DllImport("User32.dll", EntryPoint = "PostMessage")]
        private static extern int PostMessage(IntPtr hWnd, uint Msg, uint wParam, uint lParam);
        [DllImport("USER32.DLL", CharSet = CharSet.Auto, SetLastError = true, ExactSpelling = true)]
        private static extern bool SetForegroundWindow(IntPtr hWnd);
        [DllImport("user32", CharSet = CharSet.Auto, SetLastError = true, ExactSpelling = true)]
        private static extern bool keybd_event(byte bVk, byte bScan, int dwFlags, int dwExtraInfo);
        [DllImport("kernel32", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
        private static extern void Sleep(long dwMilliseconds);
        [DllImport("user32.dll", EntryPoint = "GetWindowText", CharSet = CharSet.Ansi)]
        public static extern bool GetWindowText(IntPtr hWnd, [OutAttribute()] StringBuilder strNewWindowName,
            Int32 maxCharCount);
        [DllImport("user32", CharSet = CharSet.Auto, SetLastError = true, ExactSpelling = true)]
        private static extern bool SetWindowText(IntPtr hwnd, string lpString);
        [DllImport("user32", CharSet = CharSet.Auto, SetLastError = true, ExactSpelling = true)]
        private static extern long EnumChildWindows(long hWndParent, long lpEnumFunc, long lParam);

        private const uint WM_KEYDOWN = 0x100;
        private const uint WM_KEYUP = 0x101;
        private const uint WM_MOUSEACTIVATE = 0x21;
        private const int KEYEVENTF_EXTENDEDKEY = 0x1;
        private const int KEYEVENTF_KEYUP = 0x2;

        private IntPtr w;
        public spotify()
        {
            w = FindWindow("SpotifyMainWindow", null);
        }

        public string Nowplaying()
        {
            StringBuilder sbWinText = new StringBuilder(256);
            GetWindowText(w, sbWinText, 256);
            return sbWinText.ToString();
        }


    }
}
