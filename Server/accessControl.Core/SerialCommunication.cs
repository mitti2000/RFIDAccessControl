using System;
using System.ComponentModel;
using System.IO.Ports;

namespace accessControl.Core
{
    public class SerialCommunication : IDisposable
    {
        private readonly SerialPort _serialPort;
        private readonly BackgroundWorker _worker;
        private Action<string> _receiveCallback;
        private bool _listening;

        public SerialCommunication(string portName, int baudRate)
        {
            _serialPort = new SerialPort(portName, baudRate);
            _worker = new BackgroundWorker();
            _worker.DoWork += (sender, args) =>
            {
                _serialPort.Open();
                while (!_worker.CancellationPending)
                {
                    var data = _serialPort.ReadLine();
                    _receiveCallback.Invoke(data);
                }
                _serialPort.Close();
                _listening = false;
            };
        }

        public void StartListening(Action<string> receiveCallback)
        {
            if (!_listening)
            {
                _receiveCallback = receiveCallback;
                _worker.RunWorkerAsync();
                _listening = true;
            }
        }

        private void StopListening()
        {
            if (_listening)
            {
                _worker.CancelAsync();
            }
        }

        public void Send(string message)
        {
            _serialPort.WriteLine(message);
        }

        public void Dispose()
        {
            StopListening();
        }
    }
}
