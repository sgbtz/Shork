# Shork
Preparación y funcionamiento.

A continuación exponemos los distintos pasos a seguir para la preparación y ejecución del programa:

#Compilación

Se ha hecho una simplificación importante del proceso de compilación haciendo uso de un archivo makefile. En primer lugar, ejecutar a través de la terminal en el directorio principal (Shork/) el comando make delete para asegurar la actualización de los ejecutables. Después, proceder a compilar ejecutando de la misma forma make o make shork, automáticamente se comprobarán los ficheros necesarios y se crearán los ejecutables client y server en las carpetas client/bin y server/bin respectivamente.

#Depuración

Para facilitar el proceso de desarrollo del proyecto se ha añadido también la instrucción make debug, la cual compila permitiendo depurar después con GDB.
La depuración se ha abreviado teniendo que ejecutar sólo los ficheros shell script debug_client.sh y debug_server.sh para empezar a depurar cliente y servidor.

#Ejecución

Como indicamos anteriormente, la compilación genera los dos archivos ejecutables client y server, los cuales pueden ejecutarse fácilmente con los ficheros shell script client.sh y server.sh respectivamente; esto se hizo para evitar tener que introducir la ruta de cada ejecutable en cada ejecución, de forma similar a como se ha hecho con la depuración.

#Funcionamiento

En esta sección explicaremos cómo funciona la aplicación, tanto la parte del cliente como la del servidor y su administración.

Al ejecutar el servidor nos encontramos en el prompt el menú de administración. En este menú podemos realizar operaciones sobre los usuarios del programa; encontramos la opción de crear un nuevo usuario, borrar un usuario existente o modificarlo. La creación o borrado de un usuario implica la creación o borrado de su respectiva carpeta privada. Para salir del menú de administración basta con introducir la opción Exit.
Sin embargo, la salida del administrador no implica el fin de la parte del servidor que trata las peticiones de los clientes, ya que estas dos partes se ejecutan en procesos diferentes.

Por otro lado, al ejecutar el cliente se muestra la fase de login; el usuario debe introducir un nombre de usuario y su contraseña hasta que ambos parámetros sean correctos. Estos se transmiten mediante una cola de mensajes al servidor, donde se crea un nuevo hilo por cada mensaje de login que le llega.
El hilo se encarga de autenticar al usuario y devolver en la cola la respuesta, OK o ERROR dependiendo de si los datos eran correctos. Si los datos introducidos son correctos, el cliente pasa a un menú donde se le pregunta si desea entrar en la carpeta compartida o en la privada; tras indicar la carpeta se muestra otro menú con las opciones para operar en la carpeta anteriormente indicada. 
Mientras tanto, en el servidor, el hilo (tras devolver OK) crea una nueva cola privada para esa conexión en concreto. Su función ahora será la de recibir las operaciones que le envía el cliente a través de dicha cola y actuar en consecuencia, enviando el resultado (si ha habido error o no) al usuario.

Las distintas operaciones que el usuario puede realizar son: mostrar los ficheros de la carpeta (comando ls), descargar un fichero, subir un fichero, borrar un fichero, cambiar de carpeta (de privada a compartida y viceversa) y salir del programa. 
Por último, para terminar la ejecución del servidor (recordar que era un proceso distinto al administrador) basta con mandar una señal de interrupción mediante CTRL-C, la cual está armada para liberar todos los recursos antes de finalizar.
