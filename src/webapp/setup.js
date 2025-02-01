var Module = {
  canvas: document.getElementById('canvas'),
  onRuntimeInitialized: function() {
    setupDragAndDrop();
  }
};

function showStatus(message, duration = 3000) {
  const status = document.getElementById('status');
  status.textContent = message;
  status.style.display = 'block';
  setTimeout(() => {
    status.style.display = 'none';
  }, duration);
}

async function processUrl(url) {
  showStatus('Downloading file...');

  // Handle GitHub URLs - convert to raw content if needed
  if (url.includes('github.com') && !url.includes('raw.githubusercontent.com')) {
    url = url.replace('github.com', 'raw.githubusercontent.com')
      .replace('/raw/refs/', '/refs/');
  }

  try {
    const response = await fetch(url, {
      mode: 'cors',
      headers: {
        'Accept': 'application/octet-stream'
      }
    });

    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    const buffer = await response.arrayBuffer();
    const uint8Array = new Uint8Array(buffer);

    // Create a virtual filesystem path for the file
    const filepath = '/downloaded.brd';

    // Write the file to Emscripten's virtual filesystem
    FS.writeFile(filepath, uint8Array);

    // Call the C++ function to handle the new file
    Module.ccall('handleDroppedFile', 'void', ['string'], [filepath]);
    showStatus('File processed successfully');
  } catch (error) {
    console.error('Error downloading file:', error);
    showStatus(`Error: ${error.message}`, 5000);
  }
}

function setupDragAndDrop() {
  async function handleDrop(event) {
    event.preventDefault();
    document.querySelector('.drop-zone').classList.remove('active');

    // Check if it's a file or a URL
    if (event.dataTransfer.files.length > 0) {
      // Handle file drop
      const file = event.dataTransfer.files[0];
      const reader = new FileReader();
      reader.onload = function(e) {
        const uint8Array = new Uint8Array(e.target.result);
        const filepath = '/uploaded.brd';

        try {
          FS.writeFile(filepath, uint8Array);
          Module.ccall('handleDroppedFile', 'void', ['string'], [filepath]);
          showStatus('File processed successfully');
        } catch (error) {
          console.error('Error processing file:', error);
          showStatus(`Error: ${error.message}`, 5000);
        }
      };
      reader.readAsArrayBuffer(file);
    } else {
      // Handle URL drop
      const items = Array.from(event.dataTransfer.items);
      for (const item of items) {
        if (item.kind === 'string' && item.type === 'text/uri-list') {
          item.getAsString(async (url) => {
            await processUrl(url);
          });
          break;
        }
      }
    }
  }

  function handleDragOver(event) {
    event.preventDefault();
    document.querySelector('.drop-zone').classList.add('active');
  }

  function handleDragLeave(event) {
    event.preventDefault();
    document.querySelector('.drop-zone').classList.remove('active');
  }

  document.addEventListener('dragover', handleDragOver);
  document.addEventListener('dragleave', handleDragLeave);
  document.addEventListener('drop', handleDrop);
}
