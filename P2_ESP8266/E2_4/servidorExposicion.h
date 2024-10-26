<html>
<head>
  <title>Exposicion Luminica</title>
  <style>
    body {
      text-align: center;
    }
  </style>
</head>
<body>
  <h1>EXPOSICION LUMINICA</h1>
  <p>Exposicion: <span id="exposicionValue"></span>%</p>
  <form>
    <label for="umbral">Umbral:</label>
    <input type="number" id="umbral" name="umbral" required min="0" max="100">
    <button type="submit">Enviar</button>
  </form>

  <script>
    // Función para obtener los datos de exposición del servidor (GET)
    function getExposicionData() {
      fetch('/readExposicion')
        .then(response => response.text())
        .then(data => {
          document.getElementById('exposicionValue').textContent = data;
        })
        .catch(error => {
          console.error('Error al obtener los datos:', error);
        });
    }

    // Manejar la sumisión del formulario (POST)
    document.querySelector('form').addEventListener('submit', (event) => {
      event.preventDefault();

      const umbral = document.getElementById('umbral').value;

      fetch('/setUmbral', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: `umbral=${umbral}`
      })
      .then(response => {
        if (response.ok) {
          console.log('Umbral establecido correctamente');
          document.getElementById('umbralValue').textContent = umbral;
        } else {
          console.error('Error al establecer el umbral');
        }
      })
      .catch(error => {
        console.error('Error al enviar el formulario:', error);
      });
    });

    // Llamar a la función para obtener los datos de exposición inicialmente y luego cada cierto tiempo
    getExposicionData();
    setInterval(getExposicionData, 1000);

  </script>
</body>
</html>

webPage = "<html><head><title>Exposicion Luminica</title><style>body {text-align: center;}</style></head>";
  webPage += "<body><h1>EXPOSICION LUMINICA</h1><p>Exposicion: <span id=\"exposicionValue\"></span>%</p>";
  webPage += "<form><label for=\"umbral\">Umbral:</label>";
  webPage += "<input type=\"number\" id=\"umbral\" name=\"umbral\" required min=\"0\" max=\"100\"><button type=\"submit\">Enviar</button></form>";
  webPage += "<script>function getExposicionData() {fetch('/readExposicion').then(response => response.text()).then(data =>{";
  webPage += "document.getElementById('exposicionValue').textContent = data;}).catch(error => {console.error('Error al obtener los datos:', error);});}";
  webPage += "document.querySelector('form').addEventListener('submit', (event) => {event.preventDefault();";
  webPage += "const umbral = document.getElementById('umbral').value;fetch('/setUmbral', {";
  webPage += "method: 'POST',headers: {'Content-Type': 'application/x-www-form-urlencoded'},body: `umbral=${umbral}`})";
  webPage += ".then(response => {if (response.ok) {console.log('Umbral establecido correctamente');";
  webPage += "document.getElementById('umbralValue').textContent = umbral;} else {console.error('Error al establecer el umbral');}})";
  webPage += ".catch(error => {console.error('Error al enviar el formulario:', error);});});";
  webPage += "getExposicionData();setInterval(getExposicionData, 1000); </script></body></html>";