#include "mygl.h"


#define MULTITHREADING

void MyGL::slot_on_show_plane_changed(bool b)
{
    showPlane = b;
    // update();
}

void MyGL::slot_set_num_threads(int t)
{
    numThreads = t;
}

void MyGL::slot_tissue_preset(int s)
{
    densityThreshold = s;
}

void MyGL::slot_on_slider_moved(int num)
{

    // If there are image files associated with this, display the image on the slider
    if(!ctScanFilePath.isEmpty())
    {
        QDir imgDir = QDir(ctScanFilePath);
        QStringList qsl; qsl.append("*.ppm");
        imgDir.setNameFilters(qsl);

        QDirIterator it(imgDir, QDirIterator::Subdirectories);
        QStringList fileList = imgDir.entryList();

        // figure out the "percentage" of the way up it is
        float percent = (float) num / 100.0f;
        float layer = maxLayers * percent;
        currLayer = std::round(layer);

        if(currLayer < 0) {
            currLayer = 0;
        } else if (currLayer >= fileList.size()) {
            currLayer = fileList.size() - 1;
        }

        QString currFile = imgDir.absolutePath().append("/").append(fileList.at(currLayer));

        QPixmap pix = QPixmap(currFile);

        emit sig_send_image(pix);
        // update();

    }

}


void MyGL::slot_on_color_checkbox_changed(bool col)
{
    isColorEnabled = col;
    // update();
}

void MyGL::slot_on_opacity_checkbox_changed(bool opa)
{
    isOpacityEnabled = opa;
    // update();
}

void MyGL::slot_on_text_changed(QString s)
{
    newFileName = "/Users/chloebrownsnyder/Desktop/Spring2017/SeniorDesign/preloaded_volumes/" + s;
}

void MyGL::slot_on_newMesh_clicked()
{
    // allow user to select a directory, set this in voxelizer
    // run the program as normal, but at end, export the indices and vertices to a file



    QString dirName = QFileDialog::getExistingDirectory(this,
                                                        tr("Open Directory"),
                                                        "/Users/chloebrownsnyder/Desktop/Spring2017/SeniorDesign/CTScanImages/",
                                                        QFileDialog::ShowDirsOnly);
    if(!dirName.isNull() && !dirName.isEmpty()){
        chunks.clear();
        ctScanFilePath = dirName;
        emit sig_send_text("Image directory successfully set");
        emit sig_set_enabled(true);
    } else {
        if(ctScanFilePath.isEmpty() || ctScanFilePath.isNull() || dirName.isNull() || dirName.isEmpty())
        {
            emit sig_send_text("No image directory selected, please try again");
            emit sig_set_enabled(false);
        }
    }
}

void MyGL::slot_startLoading()
{
    if(!ctScanFilePath.isEmpty() || !ctScanFilePath.isNull()){

        mVoxelizer = CVoxelizer();
        mVoxelizer.setTargetDirPath(ctScanFilePath);
        mVoxelizer.setDensityThreshold(densityThreshold);
        processFiles();
        emit sig_send_max_layers(maxLayers);
        emit sig_send_text("Begin loading");
        //  update();
    }
}


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent), prog_lambert(this), prog_wire(this), prog_color(this), squareplane(this), camera(Camera())
{
    setFocusPolicy(Qt::ClickFocus);

}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);

}


void MyGL::initializeGL()
{
    // Create an OpenGL context
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);


    ///TRIANGLE LINES
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);


    /// TRIANGLE LINES
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);


    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(.5, .5, .5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);


    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the wireframe shader
    prog_wire.create(":/glsl/wire.vert.glsl", ":/glsl/wire.frag.glsl");
    // Create and set up the heat map shader
    prog_color.create(":/glsl/colormap.vert.glsl", ":/glsl/colormap.frag.glsl");

    squareplane.create();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

}

void MyGL::processFiles() {

    timer.start();
    progress = 0;
    emit sig_update_progress(progress);
    emit sig_send_text(QString("Start reading CT Scans"));
    mVoxelizer.processFiles();
    std::cout << "Reading images took " << timer.elapsed() << " milliseconds" << std::endl;

    progress += 10;
    emit sig_update_progress(progress);
    emit sig_send_text(QString("Finished reading CT Scans"));


#ifndef MULTITHREADING
    createChunkVector();
#else
    createChunkVectorMT();
#endif
    progress = 100;
    emit sig_update_progress(progress);
    std::cout << "Process took " << timer.elapsed() << " milliseconds" << std::endl;
    emit sig_send_text(QString("Process complete"));
    slot_on_slider_moved(0);

}

void MyGL::resizeGL(int w, int h)
{

    focusWidth = w;
    focusHeight = h;
    camera = Camera(focusWidth, focusHeight, focusEye, focusRef, worldUp);

    glm::mat4 viewproj = camera.getViewProj();

    // Upload the projection matrix
    prog_lambert.setViewProjMatrix(viewproj);
    prog_wire.setViewProjMatrix(viewproj);
    prog_color.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

void MyGL::onCameraMove()
{
    float amount = 1.f;
    if(shiftOperator)
    {
        amount *= 10;
    }
    if(translateUp)
    {
        camera.TranslateAlongUp(amount);
    }
    if(translateDown)
    {
        camera.TranslateAlongUp(-amount);
    }
    if(translateRight)
    {
        camera.TranslateAlongRight(-amount);
    }
    if(translateLeft)
    {
        camera.TranslateAlongRight(amount);
    }
    if(translateForward)
    {
        camera.TranslateAlongLook(amount);
    }
    if(translateBackward)
    {
        camera.TranslateAlongLook(-amount);
    }
    if(rotateRight)
    {
        camera.RotateAboutUp(-amount);
    }
    if(rotateLeft)
    {
        camera.RotateAboutUp(amount);
    }
    if(rotateUp)
    {
        camera.RotateAboutRight(-amount);
    }
    if(rotateDown)
    {
        camera.RotateAboutRight(amount);
    }

}

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{

    threadsDone = QThreadPool::globalInstance()->waitForDone(1);
    if(!threadsDone)
    {
        for(unsigned long c = 0; c < chunks.size(); c++)
        {
            chunks.at(c)->create();
        }
    }

    onCameraMove();

    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the viewproj matrix
    prog_lambert.setViewProjMatrix(camera.getViewProj());
    prog_wire.setViewProjMatrix(camera.getViewProj());
    prog_color.setViewProjMatrix(camera.getViewProj());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(-1, 1, 1));
    prog_lambert.setModelMatrix(model);
    prog_color.setModelMatrix(model);
    prog_wire.setModelMatrix(model);

    glm::mat4 translate = glm::translate(model, glm::vec3(0, currLayer, 0));

    prog_lambert.setModelMatrix(translate);

    if(showPlane)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        prog_lambert.draw(squareplane);
        glDisable(GL_BLEND);
    }

    prog_lambert.setModelMatrix(model);

    if(isOpacityEnabled){
        // PLAY WITH BLEND FUNC? GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_ONE and GL_SAMPLE_COVERAGE?
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }


    for(unsigned int i = 0; i < chunks.size() ; i++) {
        CChunk* currChunk = chunks[i];
        if(currChunk->isCreated()){
            if(isColorEnabled){
                prog_color.draw(*currChunk);
            } else {
                prog_lambert.draw(*currChunk);
            }
        }
    }

}


void MyGL::progressFinishedBuildingWorld()
{
    progress += 20;
    emit sig_update_progress(progress);
    emit sig_send_text(QString("Finished building world"));
    progressStartBuildingChunks();
}

void MyGL::progressStartBuildingChunks()
{
    progress += 5;
    emit sig_update_progress(progress);
    emit sig_send_text(QString("Start building chunks"));
}

void MyGL::progressFinishedBuildingChunks()
{
    progress+= 30;
    emit sig_update_progress(progress);
    emit sig_send_text(QString("Finished building chunks"));
}

void MyGL::progressFinishCreatingChunks()
{
    progress += 20;
    emit sig_update_progress(progress);
    emit sig_send_text(QString("Finished creating chunks"));
}



void MyGL::createChunkVectorMT()
{

    QElapsedTimer worldTimer;
    QElapsedTimer chunkMemAllocTimer;
    QElapsedTimer threadTimer;

    worldTimer.start();

    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    // std::vector<CCreateChunkTask*>* chunkTasks = new std::vector<CCreateChunkTask*>();
    std::vector<CWorldTask*>* buildWorldTasks = new std::vector<CWorldTask*>();


    int totalLayers = allLayers->size();
    int idealThread = QThread::idealThreadCount();
    int incr = totalLayers / idealThread;
    // int incr = totalLayers / numThreads;

    int layer;
    int curr_ymin;
    int curr_ymax;

    // build the world. All threads will read from this world
    // go through each layer, and voxelize that layer's voxel plane

    mWorldArr = new CWorldArray(allLayers->size());
    emit sig_send_text(QString("Start building world"));

    for(int i = 0; i < totalLayers; i++) {
        // create a thread for each layer
        CWorldTask* buildTask = new CWorldTask(allLayers->at(i), mWorldArr);
        buildWorldTasks->push_back(buildTask);

    }

    // now run the threads
    for(CWorldTask* build : *buildWorldTasks)
    {
        QThreadPool::globalInstance()->start(build);
    }


    // Once the world is built, build the chunks
    if(QThreadPool::globalInstance()->waitForDone())
    {

        progressFinishedBuildingWorld();

        std::cout << "Elapsed time for building the world: " << worldTimer.elapsed() << " milliseconds" << std::endl;
        chunkMemAllocTimer.start();

        // now, allocate the memory for each chunk
        for(layer = 0; layer < totalLayers; layer += incr)
        {
            curr_ymin = layer;
            curr_ymax = layer + incr;
            if(curr_ymax > totalLayers) curr_ymax = totalLayers;

            CChunk* currChunk = new CChunk(this);
            currChunk ->setXMin(0);
            currChunk ->setXMax(512);
            currChunk ->setYMin(curr_ymin);
            currChunk ->setYMax(curr_ymax);
            currChunk ->setZMin(0);
            currChunk ->setZMax(512);
            currChunk ->setIsolevel(isolevel);

            std::vector<glm::vec4>* vertices = new std::vector<glm::vec4>();
            std::vector<GLuint>* indices = new std::vector<GLuint>();
            currChunk->setVertices(vertices);
            currChunk->setIndices(indices);

            currChunk->setWorld(mWorldArr);

            chunks.push_back(currChunk);

            // create the thread for each chunk
            CCreateChunkTask* thread = new CCreateChunkTask(currChunk);
            chunkTasks->push_back(thread);
        }

        std::cout << "Elapsed time for allocating memory for each chunk and thread: " << chunkMemAllocTimer.elapsed() << " milliseconds." << std::endl;


        threadTimer.start();
        // now actually run each thread
        for(CCreateChunkTask* task : *chunkTasks)
        {
            QThreadPool::globalInstance()->start(task);
        }


        // create each chunk
        /* if(QThreadPool::globalInstance()->waitForDone()){


            std::cout << "Elapsed time for threads to run: " << threadTimer.elapsed() << " milliseconds" << std::endl;

            progressFinishedBuildingChunks();

            for(unsigned long c = 0; c < chunks.size(); c++)
            {
                chunks.at(c)->create();
            }

            progressFinishCreatingChunks();
        }*/

        // set up slider's max value label
        maxLayers = allLayers->size();

        // set camera so that it is framed well
        camera = Camera(focusWidth, focusHeight, focusEye, focusRef, worldUp);

        std::cout << "Elapsed time for threads to run and chunk create: " << threadTimer.elapsed() << " milliseconds" << std::endl;
    }
}

void MyGL::createChunkVector()
{


    /*    std::vector<std::vector<CVoxel*>*>* allLayers = mVoxelizer.getAllLayers();
    CChunk* allLayerChunk = new CChunk(this);
    allLayerChunk ->setXMin(0);
    allLayerChunk ->setXMax(512);
    allLayerChunk ->setYMin(0);
    allLayerChunk ->setYMax(allLayers->size());
    allLayerChunk ->setZMin(0);
    allLayerChunk ->setZMax(512);

    allLayerChunk->setIsolevel(isolevel);

    std::vector<glm::vec4>* vertices = new std::vector<glm::vec4>();
    std::vector<GLuint>* indices = new std::vector<GLuint>();
    allLayerChunk->setVertices(vertices);
    allLayerChunk->setIndices(indices);

    // go through each layer, and voxelize that layer's voxel plane
    for(int i = 0; i < allLayers->size(); i++) {

        std::vector<CVoxel*> *currVoxelPlane = allLayers->at(i);
        allLayerChunk ->setWorld(&mWorld);

        for(CVoxel* v : *currVoxelPlane) {

            glm::vec4 voxPos = v->getPosition();
            glm::vec4 voxCol = v->getColor();
            int voxID = v->getID();

            mWorld.createChunkVoxelData(voxPos, voxCol, voxID);
        }
    }

    allLayerChunk->createVoxelBuffer();
    allLayerChunk->create();
    chunks.push_back(allLayerChunk);

    // after the chunk has been created, save the verts and indices to a file
    // so they can be used
    allLayerChunk->setNewFileName(newFileName);
    allLayerChunk->setCtScanFilePath(ctScanFilePath);
    allLayerChunk->exportVerticesAndIndicesToFile();
    maxLayers = allLayers->size();
*/
}


/*void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 10.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        bool moveright = true;

    } else if (e->key() == Qt::Key_Left) {
        camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        camera.TranslateAlongUp(amount);
    }
    camera.RecomputeAttributes();
   // update();  // Calls paintGL, among other things
}*/

void MyGL::keyPressEvent(QKeyEvent *e)
{

    if(e->key() == Qt::Key_C)
    {
        glm::vec3 eye = camera.eye;
        glm::vec3 ref = camera.ref;
        std::cout << eye.x << ", "<< eye.y << ", " << eye.z << std::endl;
        std::cout << ref.x << ", "<< ref.y << ", " << ref.z << std::endl;
    }

    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        rotateRight = true;
    } else if (e->key() == Qt::Key_Left) {
        rotateLeft = true;
    } else if (e->key() == Qt::Key_Up) {
        rotateUp = true;
    } else if (e->key() == Qt::Key_Down) {
        rotateDown = true;
    } else if (e->key() == Qt::Key_W) {
        translateForward = true;
    } else if (e->key() == Qt::Key_S) {
        translateBackward = true;
    } else if (e->key() == Qt::Key_D) {
        translateRight = true;
    } else if (e->key() == Qt::Key_A) {
        translateLeft = true;
    } else if (e->key() == Qt::Key_Q) {
        translateDown = true;
    } else if (e->key() == Qt::Key_E) {
        translateUp = true;
    } else if (e->key() == Qt::Key_Shift) {
        shiftOperator = true;
    }
    camera.RecomputeAttributes();
    // update();  // Calls paintGL, among other things
}

void MyGL::keyReleaseEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        rotateRight = false;
    } else if (e->key() == Qt::Key_Left) {
        rotateLeft = false;
    } else if (e->key() == Qt::Key_Up) {
        rotateUp = false;
    } else if (e->key() == Qt::Key_Down) {
        rotateDown = false;
    } else if (e->key() == Qt::Key_W) {
        translateForward = false;
    } else if (e->key() == Qt::Key_S) {
        translateBackward = false;
    } else if (e->key() == Qt::Key_D) {
        translateRight = false;
    } else if (e->key() == Qt::Key_A) {
        translateLeft = false;
    } else if (e->key() == Qt::Key_Q) {
        translateDown = false;
    } else if (e->key() == Qt::Key_E) {
        translateUp = false;
    } else if (e->key() == Qt::Key_Shift) {
        shiftOperator = false;
    }

    // update();  // Calls paintGL, among other things
}

